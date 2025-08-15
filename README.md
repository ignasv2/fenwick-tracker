# FENWICH-TRACKER — Real-Time Volume Tracker (Fenwick / Binary Indexed Tree)

A small C++ terminal app that tracks cumulative **trading volume** across **price buckets** using a **Fenwick Tree** (Binary Indexed Tree). It supports fast **point updates** and **prefix / range queries**, so you can answer:

* “What’s total volume up to price **X**?”
* “How much volume between prices **A..B**?”

-----------------------------------

## What data structure is implemented?

**Fenwick Tree (Binary Indexed Tree)**

* Operations:

  * `add(index, delta)` → point update in **O(log N)**
  * `prefix_sum(index)` → sum of `A[1..index]` in **O(log N)**
  * `range_sum(L, R)` → `prefix_sum(R) - prefix_sum(L-1)` in **O(log N)**
* Space: **O(N)**
* Implementation detail: it’s **1-based** internally (index 0 unused; valid indices are `1..n`).

**Short explainer:**
Think of an array `A[1..N]` holding volumes. The Fenwick Tree keeps another array `tree[1..N]` of **partial sums**. With a clever layout, a single update touches only `O(log N)` cells and a prefix sum reads only `O(log N)` cells—much faster than scanning the whole array each time.

-----------------------------------

## What the app does

* Reads **CSV** rows: `Timestamp,Open,High,Low,Close,Volume`
* Converts the close price to a **bucket** with `bucket = (int)Close` (e.g., `530.48 → 530`)
* Adds the row’s `Volume` to that bucket (Fenwick point update)
* Lets you query:

  * `prefix X` → total volume **≤ X**
  * `range A B` → total volume in **\[A..B]**
* After `add`/`prefix` it prints a small **“Local buckets \[i-2..i+2]”** window so you can see bucket-level values around the index you touched/queried.

![Program launch — main menu](https://raw.githubusercontent.com/ignasv2/fenwick-tracker/main/Screenshots/program-launch-screenshot.png)

-----------------------------------

## Project layout

```
FENWICH-TRACKER/
├── app/
│   └── main.cpp
├── code/
│   ├── Fenwick.h
│   └── Fenwick.cpp
├── data/
│   └── intraday.csv
└── CMakeLists.txt
```
-----------------------------------
## How to build & run

You can build with **CMake (recommended)** or with a **one-liner** compiler command (I am only showing the CMake way since that's what we stuck with during the class).

### Build with CMake (recommended)

From inside the `FENWICH-TRACKER/` folder, create a build folder and change directory into it (it might prevent errors from occuring if you delete my build folder and create a new one):

```bash
# Create a separate build directory
mkdir build
cd build

# Configure and build (defaults to Debug per CMakeLists.txt)
cmake ..
cmake --build .
```

Run the app:

* macOS/Linux:

  ```bash
  ./run_app
  ```
* Windows (MSVC multi-config):

  ```bat
  .\Debug\run_app.exe
  ```
**Where is the .exe?**  
- Visual Studio generator: `build\Debug\run_app.exe` (or `build\Release\run_app.exe`)  
- MinGW/Ninja: `build\run_app.exe`

**CSV path note for CMake builds**
`app/main.cpp` uses `load_csv_batch("../data/intraday.csv", fenwick);`.
When you run the program **from the `build/` directory**, the relative path `../data/intraday.csv` is correct (it points back to the project’s `data/` folder). A sample file has already been uploaded with Microsoft stock price that was pulled via Alpha Vantage API call on my local computer: https://www.alphavantage.co/support/#api-key.

-----------------------------------

![Screenshot of CMake and running the program](https://github.com/ignasv2/fenwick-tracker/blob/main/Screenshots/cmake-and-running-the-app.png)

-----------------------------------

## Using the app (step-by-step)

1. **Launch** the executable. You’ll first see:

```
Enter maximum index (n):
```

This is the number of **price buckets** (1..n) the tree supports.
For the provided CSV (prices \~520–556), enter **600**. At the time of pulling MSFT data, stock price was between $520 and $531ish.
This means that if you try to access the price buckets before 520 you will just see a bunch of zeros show up. So for testing purposes please stick to testing values between 520 and 535.

2. The **menu**:

```
1) add <index> <delta>
2) prefix <index>
3) range <leftIndex> <rightIndex>
4) reset
5) size
6) load CSV
0) exit
```

3. **Typical demo flow**

* Choose **`6`** (load CSV). You’ll get a summary: rows applied, out\_of\_bounds, total\_volume, etc.
* Choose **`2`**, then enter **`530`** → prints `prefix(530)` and a local buckets window.
* Choose **`3`** and enter **`520 556`** → total volume in that range.

![Screenshot of the demo in action](https://github.com/ignasv2/fenwick-tracker/blob/main/Screenshots/demo-in-action.png)

---

## CLI command reference

* **`1) add <index> <delta>`**
  Point-update the bucket at `<index>` by `<delta>` volume. Prints the new prefix and local window.

* **`2) prefix <index>`**
  Prints the cumulative volume from bucket `1` up to `<index>`, plus the local buckets window.

* **`3) range <L> <R>`**
  Prints the volume in the inclusive range `[L..R]`.

* **`4) reset`**
  Sets all buckets back to zero.

* **`5) size`**
  Prints the maximum index `n`.

* **`6) load CSV`**
  Loads the CSV at the hardcoded path (see **CSV path notes**) and applies each row to its bucket.

* **`0) exit`**
  Quit the program.

---

## CSV file notes

* Expected columns should match the Alpha Vantage API call output but can be easily tweaked to match other types of API call outputs (header row + data rows):

  ```
  Timestamp,Open,High,Low,Close,Volume
  ```

* Bucketing rule:

  ```
  bucket = (int)Close
  ```

  (e.g., `530.48 → 530`)

* Default load path in `app/main.cpp`:

  ```cpp
  load_csv_batch("../data/intraday.csv", fenwick);
  ```

  Adjust this string if you prefer running from the project root or another folder.

---

## Troubleshooting

* **All zeros after queries**

  * You haven’t added or loaded data yet; use `1) add` or `6) load CSV`.
  * You’re querying a bucket with no data (e.g., 5 or 20). With the sample CSV, try 520–535.

* **“Unable to open …” when loading CSV**

  * The working directory doesn’t match the relative path. Run from a folder where `../data/intraday.csv` is valid (CMake `build/`), or change the path to `data/intraday.csv`.

* **Large `out_of_bounds` count**

  * Your chosen `n` is too small. Restart and enter a larger `n` so buckets cover the data.

* **Range seems off**

  * `range_sum(L, R)` is **inclusive**, and the tree is **1-based**.

---

## Sample output (if you can’t run it)

```
Enter maximum index (n): 600

Fenwick Tree Demo (Binary Indexed Tree)
#################################
1) add <index> <delta>
2) prefix <index>
3) range <leftIndex> <rightIndex>
4) reset
5) size
6) load CSV
0) exit
#################################

Enter command (...) : 6
Loaded '../data/intraday.csv': rows=100, applied=100, out_of_bounds=0, bad=0, total_volume=133254812
Loaded intraday.csv into Fenwick tree

Enter command (...) : 2 530
prefix(530) = 133184746
Local buckets [528..532]:
 528: 8051087
 529: 13998854
 530: 4326292
 531: 70066
 532: 0

Enter command (...) : 3 520 556
range_sum(520..556) = 131412194

Enter command (...) : 0
Till the next time!
```

*(The numbers in the example above might differ from what you see depending on what's in the CSV file.)*

---

## References (other referrences are in the main.cpp file)
+ ZyBooks CS-2270 modules on recursion and trees
+ "Competitive Programmer’s Handbook" (Laaksonen) – Fenwick Tree section
+ VisuAlgo.net – https://visualgo.net/en
+ GeeksForGeeks – https://www.geeksforgeeks.org/binary-indexed-tree-or-fenwick-tree-2/
+ Investopedia – articles on volume analysis in trading
