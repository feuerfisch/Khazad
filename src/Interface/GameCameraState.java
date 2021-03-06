/*
 * To change this template, choose Tools | Templates
 * and open the template in the editor.
 */
package Interface;


import Game.Game;
import Map.MapCoordinate;

import com.jme3.app.Application;
import com.jme3.app.SimpleApplication;
import com.jme3.app.state.AbstractAppState;
import com.jme3.app.state.AppStateManager;

import com.jme3.material.Material;
import com.jme3.scene.Geometry;
import com.jme3.math.ColorRGBA;

import com.jme3.scene.Node;
import com.jme3.math.Vector3f;
import com.jme3.math.Plane;
import com.jme3.math.Ray;
import com.jme3.math.Vector2f;
import com.jme3.scene.shape.Sphere;

import com.jme3.collision.CollisionResult;
import com.jme3.collision.CollisionResults;
import com.jme3.input.InputManager;
import com.jme3.input.KeyInput;
import com.jme3.input.MouseInput;
import com.jme3.input.controls.ActionListener;
import com.jme3.input.controls.AnalogListener;
import com.jme3.input.controls.KeyTrigger;
import com.jme3.input.controls.MouseAxisTrigger;
import com.jme3.input.controls.MouseButtonTrigger;
import com.jme3.scene.Spatial;

import Renderer.TerrainRenderer;


/**
 *  Manages the main games parrelel projection Camera 
 *
 *  @author    Impaler
 */
public class GameCameraState extends AbstractAppState implements ActionListener, AnalogListener {

	public enum CameraMode {
		NORMAL,
		SELECT_VOLUME,
		SELECTING_VOLUME,
		SELECT_SURFACE,
		SELECTING_SURFACE
	}

	private Node rootnode;
	private Node terrainnode;
	private Node LookNode;

    private SimpleApplication app;
    private GameCamera MainCamera;
	private AppStateManager state; 

	private CameraMode CurrentMode = CameraMode.NORMAL;

	private boolean LeftDown;
	private boolean RightDown;
	private boolean MiddleDown;
	private boolean LShiftDown;
	private boolean RShiftDown;
	private boolean Shift;

	private float OldMouseX;
	private float OldMouseY;
	private float XChange;
	private float YChange;
	
	private Plane SelectionPlane = null;

	private MapCoordinate MouseLocation = new MapCoordinate();
	public MapCoordinate SelectionOrigin = new MapCoordinate();
	public MapCoordinate SelectionTerminus = new MapCoordinate();

	public VolumeSelection Volume;

	protected int SliceTop;
	protected int SliceBottom;
	protected int ViewLevels;
	
	protected int ViewMax, ViewMin;

    public GameCameraState() {
    }

    @Override
    public void initialize(AppStateManager stateManager, Application app) {
        super.initialize(stateManager, app);
        this.app = (SimpleApplication) app;
		this.state = stateManager;
		this.rootnode = this.app.getRootNode();

		TerrainRenderer rend = stateManager.getState(TerrainRenderer.class);
		this.terrainnode = rend.getTerrainNode();
				
        if (app.getInputManager() != null) {
       
            if (MainCamera == null) {
				LookNode = new Node();
				this.app.getRootNode().attachChild(LookNode);
				LookNode.setLocalTranslation(1, 2, 3);
				
				Sphere eye = new Sphere(10, 10, 0.5f);
				Geometry EyeBall = new Geometry("EyeBall", eye);
				EyeBall.setLocalTranslation(new Vector3f(0, 0, 0));
				Material mat1 = new Material(app.getAssetManager(), "Common/MatDefs/Misc/Unshaded.j3md");
				mat1.setColor("Color", ColorRGBA.White);
				EyeBall.setMaterial(mat1);
				LookNode.attachChild(EyeBall);
				LookNode.setCullHint(Spatial.CullHint.Always);										
				
                MainCamera = new GameCamera(app.getCamera(), LookNode);
            }
        }
		registerWithInput(app.getInputManager());
    }
	
	protected void ConvertMouseMovementToVector() {
		Vector2f Mouse = app.getInputManager().getCursorPosition();
		
		XChange = OldMouseX - Mouse.x;
		YChange = OldMouseY - Mouse.y;
		
		OldMouseX = Mouse.x;
		OldMouseY = Mouse.y;
	}

	protected Vector3f CreateTranslationVector() {
		Vector3f LookVector = MainCamera.TargetNode.getWorldTranslation().subtract(MainCamera.CamNode.getWorldTranslation());

		LookVector.normalizeLocal();
		float zComp = LookVector.z;
		LookVector.z = 0;
		LookVector.normalizeLocal();

		Vector3f TempUpVector = Vector3f.UNIT_Z;

		Vector3f CrossProduct = TempUpVector.cross(LookVector);
		CrossProduct.normalizeLocal();

		Vector3f Temp1 = CrossProduct.mult(-XChange).mult(MainCamera.TranslationFactor);  //
		Vector3f Temp2 = LookVector.mult(-YChange).mult(MainCamera.TranslationFactor).divide(zComp);

		XChange = YChange = 0; // Consume the Mouse movement
		return Temp1.add(Temp2);
	}

    public void onAction(String name, boolean keyPressed, float tpf) {
        if (this.isEnabled()) {
			if (name.equals("LeftClick")) {
				LeftDown = keyPressed;
				//if (!RightDown && keyPressed) {
				//}
				
				if (CurrentMode == CameraMode.SELECTING_VOLUME && !keyPressed)
					completeVolumeSelection();
					
				if (CurrentMode == CameraMode.SELECT_VOLUME && keyPressed)
					setMode(CameraMode.SELECTING_VOLUME);
			}
			
			if (name.equals("RightClick")) {
				RightDown = keyPressed;

				if (CurrentMode == CameraMode.SELECT_VOLUME) {
					setMode(CameraMode.NORMAL);
				}
				if (CurrentMode == CameraMode.SELECTING_VOLUME) {
					setMode(CameraMode.NORMAL);
				}
			}

			if (name.equals("MiddleClick")) {
				MiddleDown = keyPressed;
				if (MiddleDown) {
					LookNode.setCullHint(Spatial.CullHint.Dynamic);					
				} else {
					LookNode.setCullHint(Spatial.CullHint.Always);										
				}
			}
			
			if (name.equals("ArrowUp") && keyPressed) {
				ChangeViewLevel(1);
			}

			if (name.equals("ArrowDown") && keyPressed) {
				ChangeViewLevel(-1);
			}
			
			if (name.equals("RShift") && keyPressed) {
				RShiftDown = keyPressed;
				Shift = RShiftDown || LShiftDown;
			}

			if (name.equals("LShift") && keyPressed) {
				LShiftDown = keyPressed;
				Shift = RShiftDown || LShiftDown;
			}

        }
    }

	private void AnalogNormal(String name, float value, float tpf) {
		updateMousePosition();
			
        if (name.equals("mouseLeft")) {
			if (MiddleDown) {
				MainCamera.RotateCamera(value);
			} else {
				if (RightDown)
					MainCamera.TranslateCamera(CreateTranslationVector());				
			}
        } else if (name.equals("mouseRight")) {
			if (MiddleDown) {
				MainCamera.RotateCamera(-value);
			} else {
				if (RightDown)
					MainCamera.TranslateCamera(CreateTranslationVector());
			}
        } else if (name.equals("mouseUp")) {
			if (MiddleDown) {
				MainCamera.PitchCamera(value);
			}else{
				if (RightDown)
					MainCamera.TranslateCamera(CreateTranslationVector());
			}
        } else if (name.equals("mouseDown")) {
			if (MiddleDown) {
				MainCamera.PitchCamera(-value);
			}else{
				if (RightDown)
					MainCamera.TranslateCamera(CreateTranslationVector());
			}
        } else if (name.equals("ZoomIn")) {
			if (MiddleDown) {
				ChangeViewLevel(-1);				
			} else {
				MainCamera.zoomCamera(value);
			}
        } else if (name.equals("ZoomOut")) {
			if (MiddleDown) {
				ChangeViewLevel(1);
			} else {
				MainCamera.zoomCamera(-value);
			}
        }		
	}
	
	private void AnalogSelectingVolume(String name, float value, float tpf) {

		if (Shift) { // Z axis stretching
			Vector3f LookVector = MainCamera.TargetNode.getWorldTranslation().subtract(MainCamera.CamNode.getWorldTranslation());

			LookVector.normalizeLocal();
			float zComp = LookVector.z;
			LookVector.z = 0;
			LookVector.normalizeLocal();

			//Vector3f TempUpVector = Vector3f.UNIT_Z;

			//Vector3f CrossProduct = TempUpVector.cross(LookVector);
			//CrossProduct.normalizeLocal();

			//Vector3f Temp1 = CrossProduct.mult(-XChange).mult(MainCamera.TranslationFactor);  //
			Vector3f Temp2 = LookVector.mult(-YChange).mult(MainCamera.TranslationFactor).divide(zComp);
			
		} else { // XY plane intersection
			Ray ray = MainCamera.getMouseRay(app.getInputManager().getCursorPosition());
			Vector3f IntersectLocation = new Vector3f();
			ray.intersectsWherePlane(SelectionPlane, IntersectLocation);

			SelectionTerminus.Set((int) IntersectLocation.x, (int) IntersectLocation.y, (int) SelectionOrigin.Z);
			Volume.SetSize(SelectionOrigin, SelectionTerminus);
		}

        if (name.equals("mouseLeft")) {
			if (MiddleDown) {
				MainCamera.RotateCamera(value);
			} else {
				if (RightDown)
					MainCamera.TranslateCamera(CreateTranslationVector());				
			}
        } else if (name.equals("mouseRight")) {
			if (MiddleDown) {
				MainCamera.RotateCamera(-value);
			} else {
				if (RightDown)
					MainCamera.TranslateCamera(CreateTranslationVector());
			}
        } else if (name.equals("mouseUp")) {
			if (MiddleDown) {
				MainCamera.PitchCamera(value);
			}else{
				if (RightDown)
					MainCamera.TranslateCamera(CreateTranslationVector());
			}
        } else if (name.equals("mouseDown")) {
			if (MiddleDown) {
				MainCamera.PitchCamera(-value);
			}else{
				if (RightDown)
					MainCamera.TranslateCamera(CreateTranslationVector());
			}
        } else if (name.equals("ZoomIn")) {
            MainCamera.zoomCamera(value);
        } else if (name.equals("ZoomOut")) {
            MainCamera.zoomCamera(-value);
        }		
		
	}

    public void onAnalog(String name, float value, float tpf) {
		ConvertMouseMovementToVector();
		switch (CurrentMode) {
			case NORMAL:
				AnalogNormal(name, value, tpf);
				break;
				
			case SELECT_VOLUME:
				AnalogNormal(name, value, tpf);
				break;

			case SELECTING_VOLUME:
				AnalogSelectingVolume(name, value, tpf);
				break;
		}
    }

    public void registerWithInput(InputManager inputManager) {
        String[] inputs = {"LeftClick", "RightClick", "MiddleClick", "mouseDown", "mouseUp", "mouseLeft", "mouseRight", "ZoomIn", "ZoomOut", "ArrowUp", "ArrowDown", "RShift", "LShift"};

        inputManager.addMapping("mouseDown", new MouseAxisTrigger(1, true));
        inputManager.addMapping("mouseUp", new MouseAxisTrigger(1, false));
        inputManager.addMapping("ZoomIn", new MouseAxisTrigger(2, true));
        inputManager.addMapping("ZoomOut", new MouseAxisTrigger(2, false));
        inputManager.addMapping("mouseLeft", new MouseAxisTrigger(0, true));
        inputManager.addMapping("mouseRight", new MouseAxisTrigger(0, false));
        
		inputManager.addMapping("LeftClick", new MouseButtonTrigger(MouseInput.BUTTON_LEFT));
        inputManager.addMapping("RightClick", new MouseButtonTrigger(MouseInput.BUTTON_RIGHT));
        inputManager.addMapping("MiddleClick", new MouseButtonTrigger(MouseInput.BUTTON_MIDDLE));

        inputManager.addMapping("ArrowUp", new KeyTrigger(KeyInput.KEY_UP));
        inputManager.addMapping("ArrowDown", new KeyTrigger(KeyInput.KEY_DOWN));
        inputManager.addMapping("RShift", new KeyTrigger(KeyInput.KEY_RSHIFT));
        inputManager.addMapping("LShift", new KeyTrigger(KeyInput.KEY_LSHIFT));
		
        inputManager.addListener(this, inputs);
    }

	public void unregisterInput(InputManager inputManager) {
		
	}
	
	public void updateMousePosition() {
		TerrainRenderer rend = this.app.getStateManager().getState(TerrainRenderer.class);
		this.terrainnode = rend.getTerrainNode();

		Ray ray = MainCamera.getMouseRay(app.getInputManager().getCursorPosition());
		Vector3f IntersectLocation = new Vector3f();

		if (terrainnode != null) {
			CollisionResults results = new CollisionResults();		
			terrainnode.collideWith(ray, results);

			if (results.size() > 0) {
				// The closest collision point is what was truly hit:
				CollisionResult closest = results.getClosestCollision();

				Vector3f contact = closest.getContactPoint();
				Vector3f normal = closest.getContactNormal();
				IntersectLocation = contact.subtract(normal.mult(.001f));

			} else {	
				ray.intersectsWherePlane(new Plane(Vector3f.UNIT_Z, getSliceTop()), IntersectLocation);	
			}
			int x = Math.round(IntersectLocation.getX());
			int y = Math.round(IntersectLocation.getY());
			int z = Math.round(IntersectLocation.getZ());
			MouseLocation.Set(x, y, z);
		}
	}

	public MapCoordinate getMouseLocation() {
		return MouseLocation;
	}

	public void setMode(CameraMode newMode) {
		if (CurrentMode != newMode) {
			CurrentMode = newMode;
			
			if (CurrentMode == CameraMode.SELECTING_VOLUME) {
				SelectionOrigin.copy(MouseLocation);
				SelectionTerminus.copy(MouseLocation);
				Volume = new VolumeSelection(SelectionTerminus, SelectionOrigin);
				
				SelectionPlane = new Plane(Vector3f.UNIT_Z, MouseLocation.Z);
				//this.rootnode.detachChild(SelectionBox);	
			}
			
			if (CurrentMode == CameraMode.NORMAL) {
				SelectionPlane = null;
			}
		}
	}
	
	public void completeVolumeSelection() {
		Game game = state.getState(Game.class);
		game.VolumeSelectionCompleted(Volume);
		Volume = null;

		setMode(CameraMode.SELECT_VOLUME);
	}
	
	public  void SetViewSize(int max, int min) {
		ViewMax = max;
		ViewMin = min;
	}

	public void ChangeViewLevel(int Change) {
		if (Change != 0) {
			if(SliceTop + Change > ViewMax) {
				Change = SliceTop - ViewMax;
			}
			if(SliceBottom + Change < ViewMin) {
				Change = SliceBottom - ViewMin;
			}
			SliceTop += Change;
			SliceBottom += Change;
			ViewLevels = SliceTop - SliceBottom;

			//TargetNode.move(0, 0, Change);
			//CamNode.move(0, 0, Change);		
		}
	}

	public void SetSlice(int newTop, int newBottome) {
		SliceTop = newTop;
		if(SliceBottom >= SliceTop)
			SliceBottom = SliceTop - 1;

		SliceBottom = newBottome;
		if(SliceTop <= SliceBottom)
			SliceTop = SliceBottom + 1;

		ViewLevels = SliceTop - SliceBottom;
	}

	public void SetSliceTop(int newValue) {
		//TargetNode.move(0, 0, newValue - SliceTop);
		//CamNode.move(0, 0, newValue - SliceTop);

		SliceTop = newValue;
		if(SliceBottom >= SliceTop)
			SliceBottom = SliceTop - 1;
		
		ViewLevels = SliceTop - SliceBottom;
	}

	public void SetSliceBottom(int newValue) {
		//TargetNode.move(0, 0, newValue - SliceBottom);
		//CamNode.move(0, 0, newValue - SliceBottom);
		
		SliceBottom = newValue;
		if(SliceTop <= SliceBottom)
			SliceTop = SliceBottom + 1;
		
		ViewLevels = SliceTop - SliceBottom;
	}

	public int getSliceTop() {
		return SliceTop;
	}

	public int getSliceBottom() {
		return SliceBottom;
	}
   
    @Override
    public void cleanup() {
        super.cleanup();

        unregisterInput(app.getInputManager());        
    }
}

