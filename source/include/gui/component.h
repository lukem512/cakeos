/*CakeOS */

#ifndef COMPONENT_H
#define COMPONENT_H

typedef int (*callback_type_t)(int x, int y);

typedef struct component
{
      int x;
      int y;
      int width;
      int height;
      callback_type_t onMouseClick;
} component_t;

#endif
