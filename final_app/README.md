## Iteration 2 - Features

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

To create a chart:
<ul>
<li>Modify model.hpp and model.cpp as follows:</li>
  <ol>
    <li>In model.hpp add the variable you want to use to store the chart data under private and create a public getter for it</li>
    <li>In model.cpp, construct the variable in the method `dataProcessing()` in the main loop</li>
  </ol>
<li>Modify tab1.hpp and tab1.cpp and window.cpp follows:</li>
  <ol>
    <li>Follow the example in tab1.hpp and tab1.cpp to create new charts for your tab, note the modifications to the `createWidgets()`, `createLayout()`, `createCharts()` and `updateCharts()` methods</li>
    <li>`createCharts()` just creates dummy charts with no data, these will be updated on each data load</li>
    <li>In window.cpp add `yourTab->updateCharts();` below  `tab1->updateCharts()` in the `loadData()` method</li>
  </ol>
</ul>
Add charts one at a time and always recompile the program to see the changes.
As always, wrap all user facing strings in `tr()` to allow for easy translation in the future.

TODO:
- Complete all pages with charts
- Make prettier: better colours, better formatting, better chart layout, responsivity - ensure colour accessibility in light and dark modes
- Add tooltips where appropriate
- Add help menu
- Add toolbar at bottom saying which file is currently loaded
- Translate all user facing strings to French