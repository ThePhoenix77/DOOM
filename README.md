# DOOM

# Integrated Roadmap: Mandatory + Bonus (Completed in 4 Weeks)
## Week 1: Project Setup + Initial Features

Goal: Set up the development environment, parse the map, and start basic ray-casting rendering.

Tasks:

    *  Project Setup:
        - Install MiniLibX, GCC, and any required libraries.
        
    *  Map Parsing:
        - Develop the parser to handle .cub files: validate file structure, extract wall textures, floor/ceiling colors, and the map layout.
        - Handle errors (e.g., invalid characters, unclosed maps).
        
    *  Ray-Casting Implementation:
        - Start basic ray-casting logic to render a 3D view of the maze.
        - Create the initial 2D map rendering (for testing purposes, a simple static version).

Expected Completion: End of Week 1


## Week 2: Movement + Collision Detection + Minimap

Goal: Implement player movement, wall collisions, and the minimap.

Tasks:

    *  Movement:
        - Implement basic player movement with W, A, S, D for forward/backward and strafing.
        - Implement camera rotation with arrow keys.
        
    *  Collision Detection:
        - Ensure that the player cannot move through walls.
        
    *  Minimap:
        - Implement a 2D minimap that shows the layout of the map and the player’s position.
        - Update the minimap dynamically as the player moves.

Expected Completion: End of Week 2


## Week 3: Textures, Colors, Animated Sprites + Doors

Goal: Add textures, floor/ceiling colors, animated objects, and interactive doors.

Tasks:

    *  Textures and Colors:
        - Load and apply textures to walls based on orientation (North, South, East, West).
        - Implement floor and ceiling color rendering using RGB values (from the .cub file).
        
    *  Animated Sprites:
        - Add animated objects or sprites that can be displayed in the 3D world.
        - Implement basic sprite animation (e.g., flickering lights or collectible items).
        
    *  Doors (Bonus):
        - Implement doors that can open and close when the player interacts with them.
        - Create a mechanism to toggle door states (e.g., by pressing a specific key or within a certain range of the door).

Expected Completion: End of Week 3


## Week 4: Final Features + Error Handling + Polishing

Goal: Complete all features, ensure error handling is robust, and optimize the project.

Tasks:

    *  Error Handling:
        - Handle invalid or corrupted .cub files gracefully.
        - Provide clear error messages when something is wrong with the file or configuration.
        
    *  Smooth Window Management:
        - Ensure that the program handles window focus changes, minimizing, and closing without crashes.
        - Implement window closing with the red cross button and ESC key.
        
    *  Testing & Debugging:
        - Test all implemented features thoroughly, including movement, ray-casting, collision detection, textures, doors, and minimap.
        - Fix any bugs or performance issues that arise during testing.
        
    *  Optimization:
        - Optimize the code for performance (e.g., reducing redundant calculations, optimizing rendering speed).
        - Refactor code for clarity and maintainability.
        
    *  Final Touches:
        - Clean up the project and ensure it adheres to Norminette standards.
        - Puuuuuusshhhhhhhhhhhhhhhhhhh

Expected Completion: End of Week 4 (Project Completed Inchaa Allah)
