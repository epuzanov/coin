#ifndef COIN_SOPATHSWITCH_H
#define COIN_SOPATHSWITCH_H

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

#include <Inventor/nodes/SoSubNode.h>
#include <Inventor/nodes/SoGroup.h>
#include <Inventor/fields/SoSFPath.h>

class COIN_DLL_API SoPathSwitch : public SoGroup {
  typedef SoGroup inherited;

  SO_NODE_HEADER(SoPathSwitch);

public:
  static void initClass(void);

  SoPathSwitch(void);
  SoPathSwitch(int numchildren);

  SoSFPath path;

  virtual void doAction(SoAction * action);
  virtual void GLRender(SoGLRenderAction * action);
  virtual void pick(SoPickAction * action);
  virtual void getBoundingBox(SoGetBoundingBoxAction * action);
  virtual void handleEvent(SoHandleEventAction * action);
  virtual void getMatrix(SoGetMatrixAction * action);
  virtual void search(SoSearchAction * action);
  virtual void getPrimitiveCount(SoGetPrimitiveCountAction * action);

protected:
  virtual ~SoPathSwitch();

private:
  void commonConstructor(void);
};

#endif // !COIN_SOPATHSWITCH_H
