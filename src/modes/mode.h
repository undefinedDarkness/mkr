#ifndef MODES_H
#define MODES_H

#include "../basic.h"
// void async_update_progress(float,void*);

typedef struct {
  const char *label;
  GIcon *icon; // gtk specific
  void *metadata;
  int id;
} Result;

// bool async_insert_item_single(struct InsertData *i);

struct State;
typedef struct {
  void (*update_progress)(float v, struct State *);
  void (*insert_custom_item)(GList*, struct State*);
  void (*insert_item)(GList *, struct State *);
  struct State *data;
} Api_t;

#define API Api_t *api

enum ModeType {
  COMMAND = 1,
  PROGRESS = 4, // display a progress bar after command is sent
  
  ITEMS = 2,
  AUTOCLEAN = 8, // naivley try to cleanup after mode NOTE: BAD BAD BAD
  HAS_PREVIEW = 16, // this will give a preview widget
  
  ONLY_PREVIEW = 32 // no items to display, no commands to pass
};

typedef struct {
  struct {
    char *symbol; 
    enum ModeType type; // Flags + Type go here
  } metadata;
  
  char *label; // Must be uppercase and unique
  
  void (*generate)(API);
  GtkWidget* (*preview)(Result*);

  union {
    void (*execute)(const char *, Result); // Type = ITEMS
    void (*execute_command)(const char *, API); // Type = COMMAND
  };

} Mode;

void app_mode_generate(API);
void app_mode_execute(const char *const, Result);

void emoji_generate(API);
void emoji_execute(const char *, Result);

void font_generate(API);
GtkWidget* font_preview(Result*);
void font_execute(const char*, Result); 

void todo_generate(API);

GtkWidget* dashboard(void*);

void dl_execute(const char *url, API);

#endif
