/**************************************************************************\
 *
 *  This file is part of the Coin 3D visualization library.
 *  Copyright (C) 1998-2002 by Systems in Motion.  All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License
 *  ("GPL") version 2 as published by the Free Software Foundation.
 *  See the file LICENSE.GPL at the root directory of this source
 *  distribution for additional information about the GNU GPL.
 *
 *  For using Coin with software that can not be combined with the GNU
 *  GPL, and for taking advantage of the additional benefits of our
 *  support services, please contact Systems in Motion about acquiring
 *  a Coin Professional Edition License.
 *
 *  See <URL:http://www.coin3d.org> for  more information.
 *
 *  Systems in Motion, Prof Brochs gate 6, 7030 Trondheim, NORWAY.
 *  <URL:http://www.sim.no>.
 *
\**************************************************************************/

/*!
  \class SoMatrixTransform SoMatrixTransform.h Inventor/nodes/SoMatrixTransform.h
  \brief The SoMatrixTransform class is a transformation node.
  \ingroup nodes

  This class is the most flexible transformation node, as you can use
  it to accumulate any kind of transformation matrix on top of the
  current model transformation matrix.

  \sa SoTransform
*/

#include <Inventor/nodes/SoMatrixTransform.h>
#include <Inventor/nodes/SoSubNodeP.h>

#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoGLRenderAction.h>
#include <Inventor/elements/SoModelMatrixElement.h>

/*!
  \var SoSFMatrix SoMatrixTransform::matrix
  The transformation matrix. Defaults to the identity matrix.
*/


// *************************************************************************

SO_NODE_SOURCE(SoMatrixTransform);

/*!
  Constructor.
*/
SoMatrixTransform::SoMatrixTransform(void)
{
  SO_NODE_INTERNAL_CONSTRUCTOR(SoMatrixTransform);

  SO_NODE_ADD_FIELD(matrix, (SbMatrix::identity()));
}

/*!
  Destructor.
*/
SoMatrixTransform::~SoMatrixTransform()
{
}

// Doc from superclass.
void
SoMatrixTransform::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoMatrixTransform, SO_FROM_INVENTOR_1|SoNode::VRML1);
}

// Doc from superclass.
void
SoMatrixTransform::doAction(SoAction * action)
{
  if (!this->matrix.isIgnored()) {
    SoModelMatrixElement::mult(action->getState(), this,
                               this->matrix.getValue());
  }
}

// Doc from superclass.
void
SoMatrixTransform::GLRender(SoGLRenderAction * action)
{
  SoMatrixTransform::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoMatrixTransform::getBoundingBox(SoGetBoundingBoxAction * action)
{
  SoMatrixTransform::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoMatrixTransform::callback(SoCallbackAction * action)
{
  SoMatrixTransform::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoMatrixTransform::getMatrix(SoGetMatrixAction * action)
{
  SbMatrix m = this->matrix.getValue();
  action->getMatrix().multLeft(m);

  SbMatrix mi = m.inverse();
  action->getInverse().multRight(mi);
}

// Doc from superclass.
void
SoMatrixTransform::pick(SoPickAction * action)
{
  SoMatrixTransform::doAction((SoAction *)action);
}

// Doc from superclass.
void
SoMatrixTransform::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoMatrixTransform::doAction((SoAction *)action);
}
