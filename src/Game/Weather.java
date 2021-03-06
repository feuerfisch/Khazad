/* Copyright 2010 Kenneth 'Impaler' Ferland

This file is part of Khazad.

Khazad is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Khazad is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Khazad.  If not, see <http://www.gnu.org/licenses/> */

package Game;

import com.jme3.light.DirectionalLight;
import com.jme3.math.ColorRGBA;
import com.jme3.scene.Node;
import com.jme3.math.Vector3f;
import com.jme3.math.FastMath;
import com.jme3.math.Quaternion;

/**
 * Holds the state of and simulates the weather and Climate of the Games local 
 * map area.  Updates once per minute, currently controls the direction sunlight
 * 
 * @author Impaler
 */
public class Weather extends Temporal {
	
	DirectionalLight Sun;
	Quaternion Rotation;
	Vector3f SunVec;
	Node SunNode;
	ColorRGBA Suncolor = ColorRGBA.White;

	public Weather() {
		Rotation = new Quaternion();
		Rotation.fromAngleAxis(1 * FastMath.PI / 180f, new Vector3f(0, 1, 0));
		
		SunNode = new Node();
		SunVec = new Vector3f();
		
		Sun = new DirectionalLight();
		Sun.setColor(Suncolor.mult(0.6f));
	}
	
	public void AttatchSun(Node TerrainNode) {
		SunVec.set(0, 0, -1);
		SunVec.normalizeLocal();
		Sun.setDirection(SunVec);

		TerrainNode.addLight(Sun);
	}
	
	public long Wake(long CurrentTick) {		
		if (CurrentTick >= WakeTick) {
			
			// Day Rotation
			SunVec = Rotation.mult(SunVec);
			Sun.setDirection(SunVec);
			float Z = SunVec.z;
			if (Z > 0) {
				Sun.setColor(Suncolor.mult(0.0f));
			} else {
				Sun.setColor(Suncolor.mult((0.6f) * (Z * -1)));				
			}
			
			WakeTick = CurrentTick + 720;
		}
		return WakeTick;		
	}
}
