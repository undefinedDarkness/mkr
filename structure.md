# Structure of a Mode

```c
typedef struct {
   struct {
	   char* symbol; // Plain text symbol to show in header TODO: allow images
		enum ModeType type; // Type + Flags
	} metadata;
	char* label; 
	void (*generate)(void (*)(GList*,void*), void*);	
	union {
		void (*execute)(const char*, Result);
		void (*execute_command)(const char*);
		void (*execute_command_with_progress)(const char*,void*);
	};
} Mode;
```
`label` must be a ascii string in upper case containing a unique name for your mode

### Result
```c
typedef struct {
	const char* label;
	GIcon* icon; // gtk specific
	void* metadata;
	int id;
} Result;
```
Currently, `id` is not used, the rest are as you would expect.

### Type
`type` can be one of:
- COMMAND: PROGRESS
- ITEMS

`COMMAND` takes a plain text input and executes or executes with a progress bar
Where `PROGRESS` is an optional flag if the plugin intends to show a progress bar
For these modes, the `generate` function is never called
`execute_command` or `execute_command_with_progress` will be called on enter, depending on flags set.

`ITEMS` produces a list of items to be selected from, at the moment search is handled internally
`execute` will be called on selection with the label of the Result and the Result itself
- The GList* generated from `generate()` will be freed but the data within won't be.

### Future Changes
`execute_command_with_progress` and `execute_command` join into one function,
Instead of callbacks being passed from the API side, a single API structure is passed whenever necessary to contain all callbacks
