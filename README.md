# PainEngine

As my final project I'm uploading an in-progress example scene in my game engine I've called "PainEngine" because of how much I miss using Unity.
Use WASD to move, Q to jump and E to shoot a raycast out.

I created a game engine built on an entity-component system, where entities can be instantiated and component scripts can be attached to them. All entities have a transform component for their translation, rotation and scale. All components can have an awake() function called when they're first instantiated, start() function which is called at the beginning of the game, update(float delta) and lateupdate(float delta) functions that are called before and after every frame. Components can created and retrieved using the addComponent<>() and getComponent<>() functions. Entities can be retrieved through the Component::entity field. What makes this system so useful is that many objects and scripts quickly and easily be added without much chance of any systems breaking down.

    Universal components:
        Transform: Required component attached to every entity. Contains information regarding position, rotation and scale values.

        Entity: Entities can be attached to other entities as components and can calculate their cumulative transform values for use in other components.

        Camera: Contains information for FOV and scaling factor. The render view is placed at the position and rotation of an entity which has this component attached.

        Collider: Attaches a shape representing a collider to an entity. These shapes are translated, rotated and scaled by the cumulative transform of the entity and its parents that the collider is attached to. Helper functions for finding the closest point, whether or not the shape contains a point and creating AABBs are written for each shape but aren't all yet used. Axis-aligned bounding boxes are generated for every collider every frame if they've moved. Colliders are pretty much or completely finished for:
            - Sphere
            - Capsule
            - AABB
            - Box
            - Finite and infinite planes exist, but have some work to go.
        Raycasts currently work against spheres, AABBs and boxes. The closest point of intersection, normal of the surface hit and the collider hit are returned. Raycasts check layer bitmasks and AABB overlap tests before running to be as efficient as possible. Spherecasts against most basic primitives will be added in the future using geometry, but for more complex objects and capsulecasts I'll be implementing the Gilbert-Johnson-Keerthi and the Expanding Polytope algorithms. Colliders and their AABBs are drawn as outlines if their drawgizmos field is set to 1.
        
        Mesh: Attaches a mesh that will be drawn every frame to the entity. This mesh is also transformed by the cumulative transform of the entity and its parents. Meshes have fields for their model (explained later), texture ID and color. They can also just draw a teapot or the AR-wing I used in my previous homeworks for testing purposes. Models can be loaded from .gltf and .bin files using JsonCPP. I'm using the .bmp loader we were provided in class for textures. While I didn't have time to implement decoding and using skinning included in these files, the basic premise is that joints, which each have their own transformation matrix, are contained in a hierarchy which can be used to combine their transforms. Vertices are assigned weights for each of these joints to determine how much influence they have over them. Animations are combinations of different joint transformation matrices. This is first up on my list of features to add after finals week.

    Single-use or otherwise less important components:
        CameraController: Allows controlling an attached camera component with the mouse as a first-person view.

        PlayerController: Allows movement using WASD to move and Q to jump. Adds gravity and a velocity to the entity. As a placeholder until I add a capsule depth penetration test against the other primitive shapes, uses a vertical raycast to determine vertical collisions. If this raycast hits an entity with a sawblade component attached to it, this sends the player entity back to the origin. Teleports the player entity back to the world origin if it falls too far.

        EntityRotator: Rotates an object around the world origin and spins it around to show it off. Also changes its mesh color over time if the entity has one.

        NeonSign: Contains a reference for two different textures and switches between them.

        RaycastTester: When E is pressed from an entity with this, it shoots a raycast out from its position adn rotation, and prints the name of the entity it hits, if any, to console. Draws a teapot at the point where it hits.

        Sawblade: Spins an object and moves it back and forth.

- Some notes:
    The functions of the quaternion class are mostly borrowed from online sources.
    My raycast->sphere function is borrowed from an online source.
    JsonCPP is an external library.

[Mascot](icon.png)