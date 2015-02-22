/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Renderer;

import Game.Game;

import Map.Axis;
import Map.Cell;
import Map.CellCoordinate;
import Map.Direction;
import Map.GameMap;
import Map.MapCoordinate;

import PathFinding.PathFinding;
import PathFinding.MovementModality;

import com.jme3.app.Application;
import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AbstractAppState;
import com.jme3.app.state.AppStateManager;
import com.jme3.asset.AssetManager;

import com.jme3.input.KeyInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.InputManager;

import com.jme3.material.Material;
import com.jme3.math.ColorRGBA;
import com.jme3.math.Vector3f;

import com.jme3.scene.Spatial;
import com.jme3.scene.Geometry;
import com.jme3.scene.Mesh;
import com.jme3.scene.Node;
import com.jme3.scene.VertexBuffer;
import com.jme3.util.BufferUtils;

import java.util.HashMap;
import java.util.concurrent.ConcurrentHashMap;
import java.util.BitSet;
import java.util.ArrayList;

import jme3tools.optimize.GeometryBatchFactory;


/**
 *
 * @author Impaler
 */
public class PathingRenderer extends AbstractAppState implements ActionListener {

	SimpleApplication app = null;
	AppStateManager state = null;
	AssetManager assetmanager = null;

	Game game;
	PathFinding Pathing;
	Node PathingNode;

	HashMap<CellCoordinate, Node> cells;
	HashMap<Integer, Material> ZoneMaterials;

	boolean DisplayToggle = false;
	
	Vector3f [] vertices;

	@Override
    public void initialize(AppStateManager stateManager, Application app) {
		super.initialize(stateManager, app);
        this.app = (SimpleApplication) app;
		this.state = stateManager;
		this.assetmanager = app.getAssetManager();
		
		this.vertices = new Vector3f[Direction.ANGULAR_DIRECTIONS.length];
		
		for (Direction dir: Direction.ANGULAR_DIRECTIONS) {			
			vertices[dir.ordinal()] = new Vector3f(dir.ValueonAxis(Axis.AXIS_X) * MapCoordinate.HALFCUBE, dir.ValueonAxis(Axis.AXIS_Y) * MapCoordinate.HALFCUBE, dir.ValueonAxis(Axis.AXIS_Z) * MapCoordinate.HALFCUBE);
		}
		registerWithInput(app.getInputManager());
	}

	public void attachToGame(Game TargetGame) {
		this.game = TargetGame;

		PathingNode = new Node();
		this.app.getRootNode().attachChild(PathingNode);
		
		this.Pathing = PathFinding.getSinglton();
		ZoneMaterials = new HashMap<Integer, Material>();
	}

	public void onAction(String name, boolean keyPressed, float tpf) {
        if (this.isEnabled()) {
			if (name.equals("PathingRenderToggle") && keyPressed) {
				DisplayToggle = !DisplayToggle;
			}
        }
    }

	public void registerWithInput(InputManager inputManager) {
        String[] inputs = {"PathingRenderToggle"};

        inputManager.addMapping("PathingRenderToggle", new KeyTrigger(KeyInput.KEY_P));		
        inputManager.addListener(this, inputs);
    }

	public Node BuildRendering(Cell TargetCell) {

		Node PathRenderingNode = new Node();
		MovementModality Mod = new MovementModality(MovementModality.MovementType.WALK_MOVEMENT, 1, 1);
		
		CellCoordinate CellCoords = TargetCell.getCellCoordinates();
		
		for (int x = 0; x < MapCoordinate.CELLEDGESIZE; x++) {
			for (int y = 0; y < MapCoordinate.CELLEDGESIZE; y++) {

				MapCoordinate TargetCoords = new MapCoordinate(CellCoords, x, y);
				BitSet Connectivity = Pathing.getDirectionFlags(TargetCoords, Mod);
				
				int Zone = Pathing.getConnectivityZone(TargetCoords, Mod);
				Material mat = ZoneMaterials.get(Zone);
				if (mat == null) {
					mat = new Material(app.getAssetManager(), "Common/MatDefs/Misc/Unshaded.j3md");
					mat.setColor("Color", ColorRGBA.randomColor());
					ZoneMaterials.put(Zone, mat);
				}

				if (Connectivity.cardinality() > 0) {
					Mesh EdgeWires = new Mesh();
					EdgeWires.setMode(Mesh.Mode.Lines);
					EdgeWires.setLineWidth(5);
					
					ArrayList<Integer> Indexes = new ArrayList<Integer>();
					for (Direction dir: Direction.ANGULAR_DIRECTIONS) {
						if (Pathing.getEdgeCost(TargetCoords, dir, Mod) != -1) {
							Indexes.add(0);
							Indexes.add(dir.ordinal());
						}
					}

					int [] indexes = new int[Indexes.size()];
					for (int i = 0; i < Indexes.size(); i++) {
						indexes[i] = Indexes.get(i).intValue();
					}

					EdgeWires.setBuffer(VertexBuffer.Type.Position, 3, BufferUtils.createFloatBuffer(vertices));
					EdgeWires.setBuffer(VertexBuffer.Type.Index,    2, BufferUtils.createIntBuffer(indexes));
					EdgeWires.updateBound();

					Geometry Wires = new Geometry("Connection Wires", EdgeWires);		
					Wires.setMaterial(mat);		
					Wires.setLocalTranslation(new Vector3f(x, y, 0));
					PathRenderingNode.attachChild(Wires);
				}
			}
		}

		GeometryBatchFactory.optimize(PathRenderingNode, true);
		if (PathRenderingNode.getQuantity() > 0) {
			Spatial ConnectionRendering = PathRenderingNode.getChild(0);
			PathRenderingNode.setName("Connection Rendering" + CellCoords.toString());
			return PathRenderingNode;		
		} else {
			return null;
		}
	}
	
	public void RebuildDirtyCells(ConcurrentHashMap<CellCoordinate, Cell> cells) {
		for (Cell target : cells.values()) {
			CellCoordinate Coords = target.getCellCoordinates();

			TerrainRenderer Terrain = state.getState(TerrainRenderer.class);
			Node CellNode = Terrain.getCellNodeLight(Coords);
			Spatial ConnectivityNode = CellNode.getChild("Connection Rendering" + Coords.toString());	

			if (target.isPathingRenderingDirty()) {
				Node NewConnectivity = BuildRendering(target);
				if (ConnectivityNode != null)
					CellNode.detachChild(ConnectivityNode);
				if (NewConnectivity != null) {
					CellNode.attachChild(NewConnectivity);
				}
				target.setDirtyPathingRendering(false);
			}
			
			if (ConnectivityNode != null) {
				ConnectivityNode.setCullHint(Spatial.CullHint.Never);
			}
		}	
	}
	
	void HideConnectivityRendering(ConcurrentHashMap<CellCoordinate, Cell> cells) {
		for (Cell target : cells.values()) {
			CellCoordinate Coords = target.getCellCoordinates();
			TerrainRenderer Terrain = state.getState(TerrainRenderer.class);
			Node CellNode = Terrain.getCellNodeLight(Coords);
			Spatial ConnectivityNode = CellNode.getChild("Connection Rendering" + Coords.toString());

			if (ConnectivityNode != null) {
				ConnectivityNode.setCullHint(Spatial.CullHint.Always);
			}
		}
	}

	@Override
	public void update(float tpf) {	
		if (state.getState(Game.class) != null) {
			GameMap map = this.game.getMap();
			if (DisplayToggle) {
				RebuildDirtyCells(map.getCellMap());
			} else {
				HideConnectivityRendering(map.getCellMap());
			}
		}
	}
}