/*
 * v3.1 August 30, 2011 snmp/parse.h
 * Distributed per ref EMEA2011-3
 */
/*
 * FILENAME: PARSE.H
 *
 * Portions Copyright 1988, 1989 by Carnegie Mellon University All Rights Reserved.
 * Copyright 1990-96 by NetPort Software. All rights reserved.
 * Copyright 1996-2004 By InterNiche Technologies Inc. All rights reserved.
 *
 *
 * MODULE: SNMP
 *
 * ROUTINES:
 *
 * PORTABLE: yes
 */

/* Additional Copyrights: */

/* 
 * Portions Copyright 1989 by Carnegie Mellon University All Rights 
 * Reserved Permission to use, copy, modify, and distribute this 
 * software and its documentation for any purpose and without fee is 
 * hereby granted, provided that the above copyright notice appear in 
 * all copies and that both that copyright notice and this permission 
 * notice appear in supporting documentation, and that the name of 
 * CMU not be used in advertising or publicity pertaining to 
 * distribution of the software without specific, written prior 
 * permission. CMU DISCLAIMS ALL WARRANTIES WITH REGARD TO THIS 
 * SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY AND 
 * FITNESS, IN NO EVENT SHALL CMU BE LIABLE FOR ANY SPECIAL, INDIRECT 
 * OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM 
 * LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, 
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF OR IN 
 * CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE. 
 */

#ifndef _PARSE_H_
#define  _PARSE_H_   1

/*
 * parse.h
 */

/*
 * A linked list of tag-value pairs for enumerated integers.
 */
struct enum_list  {
     struct enum_list *   next;
     int   value;
     char *   label;
};

/*
 * A linked list of nodes.
 */
struct node {
     struct node *  next;
     char  label[32];  /* This node's (unique) textual name */
     int   subid;      /* This node's integer subidentifier */
     char  parent[32];/*  The   parent's textual  name */
     int   type;    /* The type of object this represents */
     struct enum_list *   enums;   /* (optional) list of enumerated integers (otherwise NULL) */
};

/*
 * A tree in the format of the tree structure of the MIB.
 */
struct tree {
     struct tree *  child_list; /* list of children of this node */
     struct tree *  next_peer;  /* Next node in list of peers */
     struct tree *  parent;
     char  label[32];  /* This node's textual name */
     int   subid;   /* This node's integer subidentifier */
     int   type;    /* This node's object type */
     struct enum_list *   enums;   /* (optional) list of enumerated integers (otherwise NULL) */
     void (*printer)();      /* Value printing function */
};

/* non-aggregate types for tree end nodes */
#define  TYPE_OTHER     0
#define  TYPE_OBJID     1
#define  TYPE_OCTETSTR     2
#define  TYPE_INTEGER      3
#define  TYPE_NETADDR      4
#define  TYPE_IPADDR       5
#define  TYPE_COUNTER      6
#define  TYPE_GAUGE     7
#define  TYPE_TIMETICKS       8
#define  TYPE_OPAQUE       9
#define  TYPE_NULL      10

struct tree * read_mib();

#endif   /* _PARSE_H_ */



