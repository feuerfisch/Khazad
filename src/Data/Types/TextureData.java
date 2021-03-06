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

package Data.Types;

import Data.DataBase;
import Data.DataLibrary;
import nu.xom.Element;

/**
 *
 * @author Impaler
 */
public class TextureData extends DataBase {

	String FilePath;
	int X, Y; 
	int Width, Height;
	
	boolean LoneTexture = true;
	
	public TextureData() {
		
	}

	public boolean LoadData(Element TextureEntry, DataLibrary Library) {
		Element Name = TextureEntry.getFirstChildElement("Name", TextureEntry.getNamespaceURI());
		Library.IndexEntry(Name.getAttributeValue("Label"), this);

		Element File = TextureEntry.getFirstChildElement("File", TextureEntry.getNamespaceURI());
		if (File != null)
			FilePath = Name.getAttributeValue("Path");
		
		Element GridLocation = TextureEntry.getFirstChildElement("GridLocation", TextureEntry.getNamespaceURI());
		if (GridLocation != null) {
			X = (short) Integer.parseInt(GridLocation.getAttribute("X").getValue());
			Y = (short) Integer.parseInt(GridLocation.getAttribute("Y").getValue());
		}

		Element SheetLocation = TextureEntry.getFirstChildElement("SheetLocation", TextureEntry.getNamespaceURI());
		if (SheetLocation != null) {
			X = (short) Integer.parseInt(SheetLocation.getAttribute("X").getValue());
			Y = (short) Integer.parseInt(SheetLocation.getAttribute("Y").getValue());
			Width = (short) Integer.parseInt(SheetLocation.getAttribute("Width").getValue());
			Height = (short) Integer.parseInt(SheetLocation.getAttribute("Height").getValue());
		}
		
		return true;
	}
	
	public boolean PostProcessing() {
		return true;
	}
}
