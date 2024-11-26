## Iteration 1 - Features

- File selection (defaults to ../../data)
- Basic file validation (excludes files with no valid data or outside the required format)
- Allows reloading files w/out resetting the app
- Tabs for different pages (placeholders)
- Table compliance highlighting (dummy values)
- Language changing (does not work for every widget, maybe custom change functions need implementing), kind of works for French but file menu and combobox need synchronised updating

Note, to generate populated translation files, run: 
```
cmake --build . --target update_translations
``` 
in the build directory