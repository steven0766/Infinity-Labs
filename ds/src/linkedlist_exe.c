#include "../include/linkedlist_exe.h"

#include <stddef.h> /* NULL */
#include <assert.h> /* assert */

node_ty *Flip(node_ty* head)
{
    node_ty* save_before_node = NULL;
    node_ty* save_after_node = NULL;
    
    assert(NULL != head);
    
    while(head)
    {
        save_after_node = head -> next;
        head -> next = save_before_node;
        save_before_node = head;
        head = save_after_node;
    }
    
    return save_before_node;
}

int HasLoop(const node_ty* head)
{
    const node_ty* slow_node = head;
    const node_ty* fast_node = head;
    
    assert(NULL != head);
    
    while (fast_node && fast_node -> next)
    {
        slow_node = slow_node -> next;
        fast_node = (fast_node -> next) -> next;
        
        if (fast_node == slow_node)
        {
            return 1;
        }
    }
    
    return 0;
}

node_ty* FindIntersection(node_ty* head_1, node_ty* head_2)
{
    node_ty* node_1 = head_1;
    node_ty* node_2 = head_2;
    
    assert(NULL != head_1);
    assert(NULL != head_2);
    
    while (node_1 != node_2)
    {
        node_1 = (NULL == node_1) ? head_2 : node_1 -> next;
        node_2 = (NULL == node_2) ? head_1 : node_2 -> next;
    }
    
    return node_1;
}
