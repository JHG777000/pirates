/*
 Copyright (c) 2014 Jacob Gordon. All rights reserved.
 
 Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
 
 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
 
 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

//File: RKMem.c
//RKMem, provides some nice macros and a funtion to make dealling with memory a little easier in C.

#include <stdlib.h>
#include <string.h>
#include "RKMem.h"

 struct RKList_node_s { struct RKList_node_s* before ; struct RKList_node_s* after ; void* data ; } ;

 struct RKList_object_s { int num_of_nodes ; RKList_node first ; RKList_node last ; } ;

 void* RKMem_Realloc_Func( void* data, size_t newsize, size_t oldsize, int NULLonError0No1Yes) {
    
    void* newdata ;
    
    newdata = malloc(newsize) ;
    
    if ( newdata != NULL ) {
        
        memcpy(newdata, data, oldsize) ;
        
        free(data) ;
        
        return newdata ;
        
    } else {
        
        if ( NULLonError0No1Yes ) {
            
            free(data) ;
            
            return NULL ;
            
        } else {
            
            return data ;
        }
    }
}

RKList RKList_NewList( void ) {
    
    RKList newlist = RKMem_NewMemOfType(RKList_object) ;
    
    newlist->num_of_nodes = 0 ;
    
    newlist->first = NULL ;
    
    newlist->last = NULL ;
    
    return newlist ;

}

RKList_node RKList_AddToList( RKList list, void* data ) {
    
    if ( list->num_of_nodes == 0 ) {
        
        list->first = RKMem_NewMemOfType(RKList_node_object) ;
        
        list->first->before = NULL ;
        
        list->first->after = NULL ;
        
        list->last = list->first ;
        
    } else {
        
        list->last->after = RKMem_NewMemOfType(RKList_node_object) ;
        
        list->last->after->before = list->last ;
        
        list->last->after->after = NULL ;
        
        list->last = list->last->after ;
    }
    
    list->last->data = data ;
    
    list->num_of_nodes++ ;
    
    return list->last ;
}

void RKList_AddNodeToList(RKList list, RKList_node node) {
    
    RKList_AddToList(list, node->data) ;
}

void RKList_MoveNodeFromListToList(RKList list_a, RKList list_b, RKList_node node) {
    
    RKList_AddNodeToList(list_b,node) ;
    
    RKList_DeleteNode(list_a, node) ;
}

void* RKList_GetData(RKList_node node) {
    
    return node->data ;
}

RKList_node RKList_GetNextNode(RKList_node node) {
    
    return node->after ;
}

RKList_node RKList_GetPreviousNode(RKList_node node) {
    
    return node->before ;
}

RKList_node RKList_GetFirstNode(RKList list) {
    
    return list->first ;
}

RKList_node RKList_GetLastNode(RKList list) {
    
    return list->last ;
}

int RKList_GetNumOfNodes(RKList list) {
    
    return list->num_of_nodes ;
}

void RKList_DeleteNode( RKList list, RKList_node node ) {
    
    if ( node == NULL ) return ;
    
    if ( ( node->before == NULL ) || ( node->after == NULL ) ) {
    
     if ( ( node->before == NULL ) && ( node->after == NULL ) ) {
            
         list->first = NULL ;
            
         list->last = NULL ;
     }
        
     if ( ( node->before == NULL ) && ( node->after != NULL ) ) {
        
         list->first = node->after ;
        
         list->first->before = NULL ;
     }
    
     if ( ( node->before != NULL ) && ( node->after == NULL ) ) {
        
         list->last = node->before ;
        
         list->last->after = NULL ;
     }
    
    }
    
    if ( ( node->before != NULL ) && ( node->after != NULL ) ) {
        
        node->before->after = node->after ;
        
        node->after->before = node->before ;
    }
    
    list->num_of_nodes-- ;
    
    free(node) ;
}

void RKList_DeleteList( RKList list ) {
    
    while ( list->first != NULL ) {
        
        RKList_DeleteNode(list,list->first) ;
    }
    
    free(list) ;
}

RKList_node RKList_GetNode( RKList list, int index ) {
    
    int i = 0 ;
    
    RKList_node node = list->first ;
    
    while ( i < index ) {
        
        if ( node == NULL ) return NULL ;
        
        node = node->after ;
        
        i++ ;
    }
    
    return node ;
}

void RKList_DeleteNodeWithIndex( RKList list, int index ) {
    
    RKList_node node = RKList_GetNode(list, index) ;
    
    RKList_DeleteNode(list, node) ;
}