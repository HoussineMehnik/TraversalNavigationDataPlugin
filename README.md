# Traversal Navigation Data Plugin

![Traversal Navigation Data Plugin](Doc/TraversalNavigationDataPlugin.png)

This plugin provides a system for baking, querying, and using traversal navigation segment data generated from the NavMesh. It is designed to support traversal gameplay features such as ledge grabs, vaults, and other navigation-based traversal actions.

---

## 1. Traversal Navigation Data Plugin

The Traversal Navigation Data Plugin extends Unreal Engine navigation by generating **Traversal Segments** from NavMesh edges. These segments can be queried at runtime and integrated directly into character traversal logic.

---

## 2. Building Traversal Navigation Data

To generate traversal navigation segments:

1. Add a **NavMesh Bounds Volume** to your level.
2. From the main menu, go to:
   **Build → Traversal Navigation → Build Traversal Navigation Data**

For a visual walkthrough, see the video below:

[![Generate Traversal Navigation Segments](Doc/GenerateSegmentsData.png)](https://www.youtube.com/watch?v=hNNRhm0xI6Q)

### Debugging Baked Segment Data

You can visualize generated traversal segments in two ways:

* Select **TraversalNavData** in the **World Outliner**, then click **Toggle Debug Draw** in the Details panel.
* Execute the console command:

```
TraversalNavData.ToggleDebug
```

---

## 3. Traversal NavData Settings

Traversal navigation generation and debug behavior can be configured via plugin settings.

![Traversal Navigation Menu](Doc/TraversalNavigationMenu.png)
![Traversal NavData Settings](Doc/TraversalNavDataSettings.png)

These settings control how traversal segments are generated, validated, and visualized during debugging.

---

## 4. Traversal NavSegment Generation Validator

The **Traversal NavSegment Generation Validator** is responsible for validating NavMesh edges and generating traversal segments.

### Default Validator

By default, a built-in validator is executed during traversal segment generation. It:

* Performs raycast checks based on character radius and height
* Validates NavMesh edges for traversal suitability
* Offsets generated segments using the NavMesh voxel cell size to ensure accurate placement near collision edges

This validator provides safe, consistent baseline validation.

### Additional Validator (Custom Validation)

You can apply an **AdditionalValidator** to perform custom validation logic on top of (or instead of) the default validator.

A Blueprint implementation is available by inheriting from:

* `TraversalNavSegmentGenerationValidator_BlueprintBase`

![Blueprint Base Validator](Doc/TraversalNavSegmentGenerationValidator_BlueprintBase.png)
![Base Validator](Doc/TraversalNavSegmentGenerationValidator_Base.png)

### Overridable Functions

Custom validators can override the following functions:

* **Validate NavMesh Edge**
  Called during generation to determine whether a NavMesh edge is valid for traversal segment creation.

  ![Validate NavMesh Edge](Doc/ValidateNavMeshEdge.png)

* **PostProcess Traversal Segment**
  Called after a traversal segment is generated, allowing modification of the final segment data (for example, assigning a segment tag or adjusting the segment position).

  ![Post Process Traversal Segment](Doc/PostProcessDoc/TraversalNavSegment.png)

### Execution Rules

* If the **DefaultValidator** is disabled in the plugin settings, only the **AdditionalValidator** is executed (if assigned).
* If both validators are enabled, the **DefaultValidator** runs first, followed by the **AdditionalValidator**.

---

## 5. Traversal NavData Subsystem

The **Traversal NavData Subsystem** provides runtime access to traversal navigation segment data and allows extending that data during gameplay.

### Available Functions

* **FindTraversalSegments**
  Searches the traversal navigation data and returns all traversal segments that intersect a specified query volume.

  ![Find Traversal Segments](Doc/FindTraversalSegments.png)

* **AddUserDefinedTraversalSegment**
  Registers a custom traversal segment at runtime, making it immediately available to traversal queries. This is intended for dynamically created or procedurally defined traversal data.

  ![Add User Defined Traversal Segment](Doc/AddUserDefinedTraversalSegment.png)

### Notes

* User-defined traversal segments are added to the **runtime octree**, not to the baked traversal navigation data.
* Currently, user-defined traversal segments **cannot be removed** once added.
* Filtering by **Tag** in `FindTraversalSegments` will be added in a future update.

---

## 6. Traversal Nav Segment Data

Each traversal segment contains structured data describing its position, orientation, and classification.

![Traversal Segment](Doc/TraversalNavSegment.png)
![Traversal Segment Struct](Doc/TraversalSegmentStruct.png)

### Gameplay Tags

* **Default Segment Tag**: `TraversalSegment.Default`
* **Custom Segment Tags**: Must use the root `TraversalSegment.*`

### Utility Functions

The following helper functions are available for working with traversal segments:

* **Find Closest Point On Traversal Segment**
  ![Find Closest Point](Doc/FindClosestPointOnTraversalSegment.png)

* **Get Traversal Segment Length**
  ![Get Segment Length](Doc/GetTraversalSegmentLength.png)

* **Get Traversal Segment Mid Point**
  ![Get Segment Mid Point](Doc/GetTraversalSegmentMidPoint.png)

* **Get Traversal Segment Direction**
  ![Get Segment Direction](Doc/GetTraversalSegmentDirection.png)

* **Get Traversal Segment Normal**
  ![Get Segment Normal](Doc/GetTraversalSegmentNormal.png)

### Segment Orientation

Each traversal segment exposes:

* **Direction**
* **Normal**
* **Binormal**

These vectors can be used to align character movement and animations to the traversal surface.

---

## 7. Game Animation Sample Integration

To integrate traversal navigation segments with the **Game Animation Sample**:

1. Add **AC_TraversalNavDataLogic** to the default character(s):
   `Content/TraversalNavPlugin/AC_TraversalNavDataLogic`
2. Replace the existing jump logic using the setup shown below.

![AC Traversal Nav Data Logic](Doc/AC_TraversalNavDataLogic.png)
![Sandbox Character CMC](Doc/SandboxCharacter_CMC.png)

This setup enables the sample character to utilize traversal navigation segment data, allowing traversal actions to respond to navigation segments instead of `AC_TraversalLogic`.

> **Note**
> Blueprints are available in the **Google Drive version**.
> Check: [https://unrealengineresources.com/traversal-navigation-data](https://unrealengineresources.com/traversal-navigation-data) or [https://unrealengineresources.com/plugins](https://unrealengineresources.com/plugins)

---

## 8. Debug Nodes

The plugin includes debug nodes to help visualize and inspect traversal navigation data during development.

![Debug Nodes](Doc/Debug.png)
