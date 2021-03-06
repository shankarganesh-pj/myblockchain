/* Copyright (c) 2015, Oracle and/or its affiliates. All rights reserved.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; version 2 of the License.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA 02110-1301  USA */

#ifndef SQL_PLUGIN_REF_INCLUDED
#define SQL_PLUGIN_REF_INCLUDED

#include "my_alloc.h"
#include "myblockchain/myblockchain_lex_string.h"

typedef struct st_myblockchain_lex_string LEX_STRING;

class sys_var;
struct st_myblockchain_plugin;
struct st_plugin_dl;

enum enum_plugin_load_option {
  PLUGIN_OFF,
  PLUGIN_ON,
  PLUGIN_FORCE,
  PLUGIN_FORCE_PLUS_PERMANENT
};

/* A handle of a plugin */

struct st_plugin_int
{
  LEX_STRING name;
  st_myblockchain_plugin *plugin;
  st_plugin_dl *plugin_dl;
  uint state;
  uint ref_count;               /* number of threads using the plugin */
  void *data;                   /* plugin type specific, e.g. handlerton */
  MEM_ROOT mem_root;            /* memory for dynamic plugin structures */
  sys_var *system_vars;         /* server variables for this plugin */
  enum_plugin_load_option load_option; /* OFF, ON, FORCE, F+PERMANENT */
};

/*
  See intern_plugin_lock() for the explanation for the
  conditionally defined plugin_ref type
*/

#ifdef DBUG_OFF
typedef struct st_plugin_int *plugin_ref;

inline st_myblockchain_plugin *plugin_decl(st_plugin_int *ref)
{
  return ref->plugin;
}
inline st_plugin_dl *plugin_dlib(st_plugin_int *ref)
{
  return ref->plugin_dl;
}
template<typename T>
inline T plugin_data(st_plugin_int *ref)
{
  return static_cast<T>(ref->data);
}
inline LEX_STRING *plugin_name(st_plugin_int *ref)
{
  return &(ref->name);
}
inline uint plugin_state(st_plugin_int *ref)
{
  return ref->state;
}
inline enum_plugin_load_option plugin_load_option(st_plugin_int *ref)
{
  return ref->load_option;
}
inline bool plugin_equals(st_plugin_int *ref1, st_plugin_int *ref2)
{
  return ref1 == ref2;
}

#else

typedef struct st_plugin_int **plugin_ref;

inline st_myblockchain_plugin *plugin_decl(st_plugin_int **ref)
{
  return ref[0]->plugin;
}
inline st_plugin_dl *plugin_dlib(st_plugin_int **ref)
{
  return ref[0]->plugin_dl;
}
template<typename T>
inline T plugin_data(st_plugin_int **ref)
{
  return static_cast<T>(ref[0]->data);
}
inline LEX_STRING *plugin_name(st_plugin_int **ref)
{
  return &(ref[0]->name);
}
inline uint plugin_state(st_plugin_int **ref)
{
  return ref[0]->state;
}
inline enum_plugin_load_option plugin_load_option(st_plugin_int **ref)
{
  return ref[0]->load_option;
}
inline bool plugin_equals(st_plugin_int **ref1, st_plugin_int **ref2)
{
  return ref1 && ref2 && (ref1[0] == ref2[0]);
}
#endif

#endif  // SQL_PLUGIN_REF_INCLUDED
