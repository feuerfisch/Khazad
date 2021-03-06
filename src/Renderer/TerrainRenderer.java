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

package Renderer;

import Map.*;
import Game.Game;
import Interface.GameCameraState;
import Data.DataManager;
import Data.Types.ColorData;

import Nifty.GUI;

import com.jme3.app.Application;
import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AbstractAppState;
import com.jme3.app.state.AppStateManager;

import com.jme3.asset.AssetManager;
import com.jme3.input.InputManager;
import com.jme3.input.KeyInput;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.ActionListener;

import com.jme3.light.AmbientLight;
import com.jme3.light.DirectionalLight;
import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;

import com.jme3.scene.control.LodControl;
import com.jme3.scene.Node;
import com.jme3.scene.Spatial;

import com.jme3.texture.Image;
import com.jme3.texture.Texture;

import java.util.concurrent.ConcurrentHashMap;
import java.util.concurrent.ExecutorService;

/**
 * Rendering class for Terrain, tracks all Scene Nodes that Terrain geometry
 * attaches too and rebuilds the geometry when Cells are dirty.  Division of
 * Terrain into light/dark allows easy hiding of surface terrain and restriction
 * of directional sunlight to appropriate surfaces.
 * 
 * @author Impaler
 */
public class TerrainRenderer extends AbstractAppState implements ActionListener {
	
	SimpleApplication app = null;
	AppStateManager state = null;
	AssetManager assetmanager = null;
	ImageManager imagemanager = null;
	
	Node terrainNode = null;
	Node sunnyterrainNode = null;
	Node darkterrainNode = null;
	
	TileBuilder builder;
	LodControl TerrainLodControler;

	ConcurrentHashMap<CellCoordinate, Node> LightCellNodeMap;
	ConcurrentHashMap<CellCoordinate, Node> DarkCellNodeMap;
	ConcurrentHashMap<Integer, Node> ZMapLight;
	ConcurrentHashMap<Integer, Node> ZMapDark;
	Material mat = null;
	
	boolean SunnyRendering = true;
	int Top; int Bottom;

	ExecutorService Executor;
	boolean DisplayToggle = true;

	public TerrainRenderer(ExecutorService Threadpool) {
		Executor = Threadpool;

		LightCellNodeMap = new ConcurrentHashMap<CellCoordinate, Node>();
		DarkCellNodeMap = new ConcurrentHashMap<CellCoordinate, Node>();
		ZMapLight = new ConcurrentHashMap<Integer, Node>();
		ZMapDark = new ConcurrentHashMap<Integer, Node>();
		builder = new TileBuilder();
	}

	@Override
	public void initialize(AppStateManager stateManager, Application app) {
		super.initialize(stateManager, app);
		this.app = (SimpleApplication) app;
		this.state = stateManager;
		this.assetmanager = app.getAssetManager();

		ImageManager Images = ImageManager.getImageManager();
		Images.Initialize(assetmanager);
		imagemanager = Images;
		
		registerWithInput(app.getInputManager());
	}
	
	public void attachToGame(Game TargetGame) {
		terrainNode = new Node();
		this.app.getRootNode().attachChild(terrainNode);
		
		darkterrainNode = new Node();
		sunnyterrainNode = new Node();
		terrainNode.attachChild(darkterrainNode);
		terrainNode.attachChild(sunnyterrainNode);

		this.TerrainLodControler = new LodControl();

		Texture tex = assetmanager.loadTexture("Textures/grass1.png");
		DataManager Data = DataManager.getDataManager();

		
		Image withBorder = imagemanager.GeneratedOverLayImage(tex.getImage(), Data.getLabelIndex("COLOR_GREEN"), Data.getLabelIndex("COLOR_BLACK"));
		tex.setImage(withBorder);

		mat = new Material(assetmanager, "Common/MatDefs/Light/Lighting.j3md");
		mat.setTexture("DiffuseMap", tex);
		
		ColorRGBA Suncolor = ColorRGBA.White;
		AmbientLight glow = new AmbientLight();
		glow.setColor(Suncolor.mult(0.8f));

		TargetGame.getWeather().AttatchSun(sunnyterrainNode);
		terrainNode.addLight(glow);		
	}
	
	public void detachFromGame() {
		terrainNode = null;
		darkterrainNode = null;
		sunnyterrainNode = null;
	}

	public void onAction(String name, boolean keyPressed, float tpf) {
        if (this.isEnabled()) {
			if (name.equals("TerrainRenderToggle") && keyPressed) {
				DisplayToggle = !DisplayToggle;
			}
		}
	}

	public void registerWithInput(InputManager inputManager) {
		String[] inputs = {"TerrainRenderToggle"};

		inputManager.addMapping("TerrainRenderToggle", new KeyTrigger(KeyInput.KEY_T));		
		inputManager.addListener(this, inputs);
	}

	public Node getCellNodeLight(CellCoordinate TargetCell) {
		Node CellNode = LightCellNodeMap.get(TargetCell);
		if (CellNode == null) {
			CellNode = new Node();	
		
			float x = (float) (TargetCell.X * MapCoordinate.CELLEDGESIZE);
			float y = (float) (TargetCell.Y * MapCoordinate.CELLEDGESIZE);

			CellNode.move(x, y, 0);

			getZNodeLight(TargetCell.Z).attachChild(CellNode);
			LightCellNodeMap.put(TargetCell, CellNode);
		}
		return CellNode;
	}

	public Node getCellNodeDark(CellCoordinate TargetCell) {
		Node CellNode = DarkCellNodeMap.get(TargetCell);
		if (CellNode == null) {
			CellNode = new Node();	
		
			float x = (float) (TargetCell.X * MapCoordinate.CELLEDGESIZE);
			float y = (float) (TargetCell.Y * MapCoordinate.CELLEDGESIZE);

			CellNode.move(x, y, 0);

			getZNodeDark(TargetCell.Z).attachChild(CellNode);
			DarkCellNodeMap.put(TargetCell, CellNode);
		}
		return CellNode;
	}

	public Node getZNodeLight(int zlevel) {
		Node targetnode = ZMapLight.get(new Integer(zlevel));
		if (targetnode == null) {

			targetnode = new Node();
			targetnode.move(0, 0, zlevel);
			ZMapLight.put(new Integer(zlevel), targetnode);
		}
		return targetnode;
	}

	public Node getZNodeDark(int zlevel) {
		Node targetnode = ZMapDark.get(new Integer(zlevel));
		if (targetnode == null) {

			targetnode = new Node();
			targetnode.move(0, 0, zlevel);
			ZMapDark.put(new Integer(zlevel), targetnode);
		}
		return targetnode;
	}

	public void RebuildDirtyCells(ConcurrentHashMap<CellCoordinate, Cell> cells) {
		for (Cell target : cells.values()) {
			CellCoordinate Coords = target.getCellCoordinates();
						
			if (target.isTerrainRenderingDirty()) {
				TerrainBuilder Builder = new TerrainBuilder(app, target, builder, mat, TerrainLodControler);
				Builder.setNodes(getCellNodeLight(Coords), getCellNodeDark(Coords));
				Executor.submit(Builder);

				target.setDirtyTerrainRendering(false);
			}
		}	
	}

	public void SetTerrainRendering(ConcurrentHashMap<CellCoordinate, Cell> cells, boolean show) {
		Spatial.CullHint hint = Spatial.CullHint.Always;
		if (show == true)
			hint = Spatial.CullHint.Dynamic;
		
		for (Cell target : cells.values()) {
			CellCoordinate Coords = target.getCellCoordinates();

			Node CellLight = getCellNodeLight(Coords);
			Node CellDark = getCellNodeDark(Coords);
			
			Spatial light = CellLight.getChild("LightGeometry Cell" + target.toString());
			Spatial dark = CellDark.getChild("DarkGeometry Cell" + target.toString());

			if (light != null)
				light.setCullHint(hint);
			if (dark != null)
				dark.setCullHint(hint);
		}
	}

	public void setSliceLevels(int top, int bottom) {
		Top = top; Bottom = bottom;
		
		for (Node targetnode : ZMapLight.values()) {
			float Z = targetnode.getLocalTranslation().getZ();
			if (Z <= Top && SunnyRendering) {
				sunnyterrainNode.attachChild(targetnode);
			} else {
				sunnyterrainNode.detachChild(targetnode);
			}
		}
		
		for (Node targetnode : ZMapDark.values()) {
			float Z = targetnode.getLocalTranslation().getZ();
			if (Z <= Top) {
				darkterrainNode.attachChild(targetnode);
			} else {
				darkterrainNode.detachChild(targetnode);
			}
		}
	}
	
	public void setSunnyVisibility(boolean newValue) {
		if (SunnyRendering != newValue) {
			SunnyRendering = newValue;
		
			for (Node target : ZMapLight.values()) {
				float Z = target.getLocalTranslation().getZ();
				if (Z > Bottom && Z <= Top && SunnyRendering) {
					target.setCullHint(Spatial.CullHint.Dynamic);
				} else {
					target.setCullHint(Spatial.CullHint.Always);				
				}
			}
		}
	}

	public Node getTerrainNode() {
		return terrainNode;
	}

	public Node getSunTerrainNode() {
		return sunnyterrainNode;
	}

	public Node getDarkTerrainNode() {
		return darkterrainNode;
	}

	@Override
	public void update(float tpf) {		
		Game game = state.getState(Game.class);
		if (game != null) {
			GameMap map = game.getMap();
			if (DisplayToggle) {
				SetTerrainRendering(map.getCellMap(), true);
				RebuildDirtyCells(map.getCellMap());
			} else {
				SetTerrainRendering(map.getCellMap(), false);
			}
			GameCameraState cam = state.getState(GameCameraState.class);
			setSliceLevels(cam.getSliceTop(), cam.getSliceBottom());

			
			GUI gui = state.getState(GUI.class);
			String TimeString = ((game.hours %24) + ":" + (game.minutes % 60) + ":" + (game.seconds % 60));
			gui.UpdateText("Timelabel", TimeString);
		}
	}
}