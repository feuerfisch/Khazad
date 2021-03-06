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

package Data;

import java.util.concurrent.ConcurrentHashMap;
import java.util.ArrayList;

import nu.xom.*;

import Data.Types.*;

/**
 *
 * @author Impaler
 */
public class DataManager {

	public static final short INVALID_INDEX = -1;

	private static DataManager instance = null;
	ConcurrentHashMap GlobalLabelMap, GlobalDataTypeMap;
			
	protected DataManager() {
		GlobalLabelMap = new ConcurrentHashMap<String, Integer>();
		GlobalDataTypeMap = new ConcurrentHashMap<String, DataLibrary>();
	}

	public static DataManager getDataManager() {
	   if(instance == null) {
		  instance = new DataManager();
	   }
	   return instance;
	}

	public ColorData getColorData(short Index)						{ return (ColorData) Colors.Entries.get(Index); }
    public int getNumColors()										{ return Colors.Entries.size(); }
    public DataLibrary getColorDataLibrary()						{ return Colors; }

    public TextureData getTextureData(short Index)					{ return (TextureData) Textures.Entries.get(Index); }
    public int getNumTextures()										{ return Textures.Entries.size(); }
    public DataLibrary getTextureDataLibrary()						{ return Textures; }

    public TextureGridData getTextureGridData(short Index)			{ return (TextureGridData) TextureGrids.Entries.get(Index); }
    public int getNumTextureGrids()									{ return TextureGrids.Entries.size(); }
    public DataLibrary getTextureGridDataLibrary()					{ return TextureGrids; }

    public TextureSheetData getTextureSheetData(short Index)		{ return (TextureSheetData) TextureSheets.Entries.get(Index); }
    public int getNumTextureSheets()								{ return TextureSheets.Entries.size(); }
    public DataLibrary getTextureSheetDataLibrary()					{ return TextureSheets; }

    public AnimationTypeData getAnimationTypeData(short Index)		{ return (AnimationTypeData) AnimationTypes.Entries.get(Index); }
    public int getNumAnimationTypes()								{ return AnimationTypes.Entries.size(); }
    public DataLibrary getAnimationTypesDataLibrary()				{ return AnimationTypes; }

    public AnimationGroupData getAnimationGroupData(short Index)	{ return (AnimationGroupData) AnimationGroups.Entries.get(Index); }
    public int getNumAnimationGroups()                              { return AnimationGroups.Entries.size(); }
    public DataLibrary getAnimationGroupsDataLibrary()				{ return AnimationGroups; }

    public ModelData getModelData(short Index)						{ return (ModelData) Models.Entries.get(Index); }
    public int getNumModels()										{ return Models.Entries.size(); }
    public DataLibrary getModelDataLibrary()						{ return Models; }

    public FontData getFontData(short Index)						{ return (FontData) Fonts.Entries.get(Index); }
    public int getNumFonts()										{ return Fonts.Entries.size(); }
    public DataLibrary getFontDataLibrary()							{ return Fonts; }

    public MaterialData getMaterialData(short Index)				{ return (MaterialData) Materials.Entries.get(Index); }
    public int getNumMaterials()									{ return Materials.Entries.size(); }
    public DataLibrary getMaterialDataLibrary()						{ return Materials; }

    public MaterialClassData getMaterialClassData(short Index)		{ return (MaterialClassData) MaterialClasses.Entries.get(Index); }
    public int getNumMaterialClasses()								{ return MaterialClasses.Entries.size(); }
	public DataLibrary getMaterialClassDataLibrary()				{ return MaterialClasses; }

    public SurfaceTypeData getSurfaceTypeData(short Index)			{ return (SurfaceTypeData) SurfaceTypes.Entries.get(Index); }
    public int getNumSurfaceTypes()									{ return SurfaceTypes.Entries.size(); }
    public DataLibrary getSurfaceTypeDataLibrary()					{ return SurfaceTypes; }

    public TreeData getTreeData(short Index)						{ return (TreeData) Trees.Entries.get(Index); }
    public int getNumTrees()										{ return Trees.Entries.size(); }
    public DataLibrary getTreeDataLibrary()							{ return Trees; }

    public BuildingData getBuildingData(short Index)				{ return (BuildingData) Buildings.Entries.get(Index); }
    public int getNumBuildings()									{ return Buildings.Entries.size(); }
    public DataLibrary getBuildingDataLibrary()						{ return Buildings; }

    // ADD New Data classes gets Here

	XMLManager XML;


    // ADD New Data classes Libraries Here

    DataLibrary Colors = new DataLibrary<ColorData>(ColorData.class, this);
    DataLibrary Textures = new DataLibrary<TextureData>(TextureData.class, this);
    DataLibrary TextureGrids = new DataLibrary<TextureGridData>(TextureGridData.class, this);
    DataLibrary TextureSheets = new DataLibrary<TextureSheetData>(TextureSheetData.class, this);
    DataLibrary AnimationTypes = new DataLibrary<AnimationTypeData>(AnimationTypeData.class, this);
    DataLibrary AnimationGroups = new DataLibrary<AnimationGroupData>(AnimationGroupData.class, this);
    DataLibrary Models = new DataLibrary<ModelData>(ModelData.class, this);
    DataLibrary Fonts = new DataLibrary<FontData>(FontData.class, this);
    DataLibrary Materials = new DataLibrary<MaterialData>(MaterialData.class, this);
    DataLibrary MaterialClasses = new DataLibrary<MaterialClassData>(MaterialClassData.class, this);
    DataLibrary SurfaceTypes = new DataLibrary<SurfaceTypeData>(SurfaceTypeData.class, this);
    DataLibrary Trees = new DataLibrary<TreeData>(TreeData.class, this);
    DataLibrary Buildings = new DataLibrary<BuildingData>(BuildingData.class, this);

	public boolean Initialize() {
		
		XML = new XMLManager();

		GlobalDataTypeMap.put("Color", Colors);
		GlobalDataTypeMap.put("Texture", Textures);
		GlobalDataTypeMap.put("TextureGrid", TextureGrids);
		GlobalDataTypeMap.put("TextureSheet", TextureSheets);
		GlobalDataTypeMap.put("AnimationType", AnimationTypes);
		GlobalDataTypeMap.put("AnimationGroup", AnimationGroups);
		GlobalDataTypeMap.put("Model", Models);
		GlobalDataTypeMap.put("Font", Fonts);
		GlobalDataTypeMap.put("Material", Materials);
		GlobalDataTypeMap.put("MaterialClass", MaterialClasses);
		GlobalDataTypeMap.put("SurfaceType", SurfaceTypes);
		GlobalDataTypeMap.put("Tree", Trees);
		GlobalDataTypeMap.put("Furniture", Buildings);


		ArrayList<String> DataFiles = new ArrayList<String>();

		// Safety null checks on documents
		Document file = XML.LoadFile("XML\\MasterFileList.xml");
		if (file != null) {
			Element root = file.getRootElement();
			Elements entries = root.getChildElements();

			for (int i = 0; i < entries.size(); i++) {
				Element DataEntry = entries.get(i);
				DataFiles.add(DataEntry.getAttribute("Path").getValue());			
			}

			file.detach();  // Free the Document

			for (String File : DataFiles) {
				LoadDataFile(File);
			}
		}

		for (Object Library: GlobalDataTypeMap.values()) {
			DataLibrary lib = (DataLibrary) Library;
			lib.PostProcessDataClass();
		}

		return true;
	}
	
	boolean LoadDataFile(String Path) {
		System.out.println("Loading " + Path);

		Document file = XML.LoadFile(Path);
		if (file != null) {
			Element root = file.getRootElement();
			Elements entries = root.getChildElements();

			for (int i = 0; i < entries.size(); i++) {
				Element DataEntry = entries.get(i);		
				String DataType = DataEntry.getLocalName();

				DataLibrary TargetLibrary = getDataTypeGroup(DataType);
				if (TargetLibrary != null) {
					TargetLibrary.LoadElement(DataEntry);
				}
			}
			file.detach();  // Free the Document
			return true;
		}
        return false;
    }

	public short getLabelIndex(String Label) {
		if (Label == null)
			return INVALID_INDEX;

		if (Label.matches("NONE"))
			return INVALID_INDEX;

		if (Label.matches(""))
			return INVALID_INDEX;

		Integer Value = (Integer) GlobalLabelMap.get(Label);

		if(Value != null) {
			return Value.shortValue();
		} else {
			System.err.println(Label + " is not in GlobalLabelMap");
			return INVALID_INDEX;
		}
	}

	DataLibrary getDataTypeGroup(String ElementType) {
		Object Library = GlobalDataTypeMap.get(ElementType);

		if(Library != null) {
			return (DataLibrary) Library;
		} else {
			System.err.println(ElementType + " is not in GlobalDataTypeMap");
			return null;
		}
	}

	void addLabel(String Label, short Index) {
		GlobalLabelMap.put(Label, (int)Index);
	}
}
