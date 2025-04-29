# ICG Wireframe

Simple application for B-Spline based 3D modeling

# Build and Run

You need Qt6.5+ and Eigen3.3 to run.

**build**

```bash
cmake -S . -B build
cmake --build build --parallel 8
```

**run**

```bash
./build/icg_wireframe
```

# Usage

You can read help dialog in application;

- Drag node to move
- Left click to add node
- Right click to delete node

  ![](assets/screens/1.png)

- Setup polyline config

  ![](assets/screens/2.png)

- Pass figure in \[-1, 1\] rect

  ![](assets/screens/3.png)

- Zoom in / Zoom out
- Use double click to reset zoom settings

  ![](assets/screens/4.png)

- Click&Drag to rotate figure

  ![](assets/screens/5.png)

- Switch from model to editor

  ![](assets/screens/6.png)

- Change color

  ![](assets/screens/7.png)
