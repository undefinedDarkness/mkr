#ifndef MODES_H
#define MODES_H

#include "../basic.h"

typedef struct {
  const char *label;
  GIcon *icon; // gtk specific
  void *metadata;
  int id;
} Result;

static Result flagResult = {0,0,0,-1};

struct State;
typedef struct {
  void (*update_progress)(float v, struct State *);
  void (*insert_custom_item)(GList *, struct State *);
  void (*insert_item)(GList *, struct State *);
  struct State *data;
} Api_t;

#define API Api_t *api

enum ModeType {
  COMMAND = 1,
  PROGRESS = 4, // display a progress bar after command is sent

  ITEMS = 2,
  CLEAN = 8,        // can clean up after itself
  HAS_PREVIEW = 16, // this will give a preview widget
  CLEAR_ON_EDIT =
      128, // clear items on edit and disable builtin search entirely..?

  ONLY_PREVIEW = 32,  // no items to display, no commands to pass
  UPDATE_ON_EDIT = 64 // call generate() when search is updated...
};

typedef struct {
  struct {
    char *symbol;
    char *desc;
    enum ModeType type; // Flags + Type go here
  } metadata;

  char *label;   // Must be uppercase and unique
  void *payload; // any data
  char key;      // activation key
                 // Preview
  GtkWidget *(*preview)(Result *);

  void (*generate)(API);

  union {
    void (*execute)(const char *, Result);      // Type = ITEMS
    void (*execute_command)(const char *, API); // Type = COMMAND
  };

  // Housekeeping
  void (*clean)(Result *);

} Mode;

void todo_generate(API);
GtkWidget *dashboard(void *);
void dl_execute(const char *url, API);

#endif
