## Iteration 1 - Features

- File selection (defaults to ../../data)
- Basic file validation (excludes files with no valid data or outside the required format)
- Allows reloading files w/out resetting the app
- Tabs for different pages (placeholders)
- Table compliance highlighting (dummy values)
- Different light and dark mode variants
- Language changing (does not work for every widget, maybe custom change functions need implementing), kind of works for French but file menu and combobox need synchronised updating

<hr> 

- No tooltips or real pages added
- Loading doesn't seem to work
- Rendering custom table styling takes significant amount of time (i.e. making all columns fit)
- Alternate table highlighting doesn't work
- Need to test fully in both light and dark mode and find apprpriate colour scheme
- Should probably define the colour scheme in some header file then import and use that as needed

Note, to generate populated translation files, run: 
```
cmake --build . --target update_translations
``` 
in the build directory
