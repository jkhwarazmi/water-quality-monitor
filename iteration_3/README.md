## Iteration 3

Basic implementation w/ SQL instead of CSV processing
Massively simplifies aggregate data processing at the expense of speed
Also always loads data on startup and initial db might be full so added custom destructor to clear it, but you can still force quit the app in other ways so probably need to refactor initial load to only load tabs after initial load

To add stuff:
- Write a SQL query for the data you want in model.cpp and model.hpp like `getComplianceStats()`
- Create the chart you want using the data in your tab as per Tab1 `updateComplianceChart()` and add it to `updateCharts()`
- Add it to window.cpp above like `tab1->updateCharts()` above `switchToTabView()` in the loadData function

To run:
```
mkdir build && cd build
```

```
cmake ..
```

```
make
```


```
./watermonitor
```

NOTE: This will not compile if your path has ANY spaces in it. To fix, just move the folder to a path without spaces, i.e. your Desktop
