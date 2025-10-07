# How to Use the Image CLI

This guide provides sample inputs for each of the menu options in the interactive command-line tool.

The image is 256x256 pixels. Coordinates are 0-indexed, so row and column values can range from 0 to 255.
The tool now uses multi-step prompts for coordinates and values to make input easier.

---

### 1. Generate New Random Image

- **Action:** Creates a new 256x256 image with random colors.
- **Input:** No input is required. Just select option `1`.

---

### 2. Adjust Brightness

- **Action:** Increases or decreases the brightness of a rectangular region.
- **Input Prompts:**
  1. `Enter Top-Left Corner (row col):`
  2. `Enter Bottom-Right Corner (row col):`
  3. `Enter brightness adjustment (-255 to 255):`
- **Sample Input:**
  ```
  0 0
  255 255
  50
  ```

---

### 3. Adjust Contrast

- **Action:** Increases or decreases the contrast of a rectangular region.
- **Input Prompts:**
  1. `Enter Top-Left Corner (row col):`
  2. `Enter Bottom-Right Corner (row col):`
  3. `Enter contrast multiplier (> 0.0):`
- **Sample Input:**
  ```
  0 0
  255 255
  2.0
  ```

---

### 4. Query Region Statistics

- **Action:** Displays the Sum, Average, Min, and Max pixel values for a rectangular region.
- **Input Prompts:**
  1. `Enter Top-Left Corner (row col):`
  2. `Enter Bottom-Right Corner (row col):`
- **Sample Input:**
  ```
  128 128
  255 255
  ```

---

### 5. Fill Region with Color

- **Action:** Fills a rectangular region with a solid color.
- **Input Prompts:**
  1. `Enter Top-Left Corner (row col):`
  2. `Enter Bottom-Right Corner (row col):`
  3. `Enter color (R G B):` (e.g., `255 0 0` for red)
- **Sample Input:**
  ```
  50 50
  100 100
  255 0 0
  ```

---

### 6. Save Image to 'output'

- **Action:** Saves the current state of the image to a new PNG file in the `output/` directory.
- **Input:** No input is required.

---

### 7. Flip Image

- **Action:** Flips the image either vertically or horizontally.
- **Input Prompts:**
  1. `Enter flip direction (0 for Vertical, 1 for Horizontal):`
- **Sample Input:**
  ```
  1
  ```

---

### 8. Rotate Image

- **Action:** Rotates the entire image 90 degrees clockwise.
- **Input:** No input is required.

---

### 9. Benchmark Performance

- **Action:** Runs a performance comparison for a full-image brightness update.
- **Input:** No input is required.

---

### 10. View Operation Log

- **Action:** Displays a log of all operations performed.
- **Input:** No input is required.

---

### 11. Reset to Original Image

- **Action:** Discards all changes and restores the image to its original state.
- **Input:** No input is required.

---

### 0. Exit

- **Action:** Exits the program.
- **Input:** No input is required.