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
  \class SoBaseKit SoBaseKit.h Inventor/nodekits/SoBaseKit.h
  \brief The SoBaseKit class is the toplevel superclass for nodekits.
  \ingroup nodekits

  Node kits are collections of nodes and other node kits (from here on
  node kits which is part of some other node kit, will only be referred 
  to as nodes or parts, see catalogs and parts), organized in a way 
  that is convenient for its use. A node kit inherits SoNode and can 
  thus be inserted into a scenegraph as any other node.
  
  The organizing of the nodes and node kits of some node kit, is done
  through catalogs. A node kit's catalog describes the nodes that can
  be members of the node kit. These members are called parts. Thus a
  node kit has a catalog describing the parts that it offers to the
  user.

  Each part in the catalog has some values saying something about the
  part itself and about the role the part plays in the scenegraph.
  Those values are:
  
  <dl>
  <dt> Name
  <dd> The name of the part.
  <dt> Type
  <dd> The part's node type.
  <dt> Default Type  
  <dd> If the part's type is an abstract superclass, this value will hold 
  the default subclass used by this part.
  <dt> Created by Default? 
  <dd> Holds \c TRUE if the part should be instantiated when the node kit 
  is instantiated, otherwise the part is kept empty until it is set by some 
  of the means applicable.
  <dt> Parent Name 
  <dd> The name of the part that is this part's parent.
  <dt> Right Sibling 
  <dd> The name of the part that is the part immediately to the right of 
  this part in the node kit scenegraph.
  <dt> Is it a List?
  <dd> Holds \c TRUE if the part is a list, otherwise it is \c FALSE. See 
  SoNodeKitListPart for more info on node kit lists.
  <dt> List Cointainer Type 
  <dd> The type of group node used to hold the items if the part is a list.
  <dt> List Element Type 
  <dd> The types of nodes that is allowed to be held by this part if the part 
  is a list.
  <dt> Is It Public? 
  <dd> Holds \c TRUE if the part should be publically available, otherwise 
  it holds \c FALSE.
  </dl>

  Node kits use lazy instantiation when it creates it's parts. This means
  that the nodes making up the parts of the nodekit only are created when
  they are needed. If the "Created by Default?" holds TRUE, then the part
  is created when the node kit itself is instantiated. If not, parts are 
  created when they are requested through SoBaseKit::getPart() or the
  SO_GET_PART() macro, or created with SoBaseKit::set(). Also, if a part is 
  set with SoBaseKit::setPart() or the SO_SET_PART() macro, any previously
  uncreated parts above the set part in the hierarchy, is created
  automatically.

  The advantages of using node kits to represent a scenegraph are many.
  \li Since a node kit collects nodes into a single unit, it becomes
      an extra abstraction layer for the application programmer. Such
      a layer can represent a model of a human being as one unit where
      subunits as arms, legs, textures, etc are contained within. Thus
      we can instantiate a model of a human by creating an instance of
      the node kit, instead of having to create a possibly large
      amount of nodes needed for such a model.
  \li A part of the node kit doesn't have one spesific setup. A shape part
      can e.g. be swapped with any other shape, since they are of the same
      type. If the node kit of a human has a part called "head" which is of
      type SoShape, it might default to a sphere. But if the programmer 
      thinks that a cube might fit better, one can set the "head" part to
      a cube instead, or maybe a face set representing a complex model of
      a head.
  \li Node kits can have as simple or as complex catalogs as needed. The
      nodes included in the node kit can, if needed, represent the
      whole range of Inventor features. One part can as easily be of a
      node kit type, making it possible to create hierarchys of node kits.
      Having a node kit of a human, it might be feasible to have sub node
      kits describing the different body parts.
  \li Node kits are an efficient way of creating scenegraphs. If some
      part of it isn't needed at the moment of node kit instantiation,
      they aren't created. Thus parts are only created when needed, either
      by the application or some other part.
  \li The application code becomes smaller and easier to read, as the node 
      kits provides simple routines for creating and setting parts.
  \li New node kits can be created through subclassing to obtain simple
      setups of scenegraphs best fitted to the application.

  The usage of a node kit is straightforward. Below follows a code
  example showing some simple SoShapeKit usage.

  \code

  #include <Inventor/Qt/SoQt.h>
  #include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
  #include <Inventor/nodekits/SoShapeKit.h>
  #include <Inventor/nodes/SoSeparator.h>
  #include <Inventor/nodes/SoCube.h>
  
  int
  main(int argc, char ** argv)
  {
    QWidget * window = SoQt::init( argv[0] );
  
    SoQtExaminerViewer * viewer = new SoQtExaminerViewer( window );
  
    // Instantiating a shape kit, by default creating a simple sphere.
    SoShapeKit * shapekit = new SoShapeKit;
    // Swapping the sphere with a cube.
    shapekit->setPart("shape", new SoCube);
    // Setting the cube to be rendered in the color red. The shape kit
    // has a SoAppearanceKit as one of it's parts. The "material" part
    // used to set the color of the shape, really belongs the 
    // appearance kit. If the SoShapeKit::set() is used, it will
    // check if some of its sub kits has a part with the name given,
    // and delegate the setting to the correct kit.
    shapekit->set("material", "diffuseColor 1 0 0");
    
    SoSeparator * newroot = new SoSeparator;
    newroot->ref();
    
    newroot->addChild(shapekit);
    
    viewer->setSceneGraph(newroot);
    
    viewer->show();
    SoQt::show( window );
    
    SoQt::mainLoop();
    delete viewer;

    newroot->unref();
    return 0;
  }
  \endcode

  The above code snippet will produce a viewer with a side view to
  the scene shown below:
  
  <center>
  <img src="http://doc.coin3d.org/images/Coin/nodekits/basekitexample.png">
  </center>

  Notice that the code needed for creating this simple shape using
  a shape kit, amounts to this:

  \code
   SoShapeKit * shapekit = new SoShapeKit;
   
   shapekit->setPart("shape", new SoCube);
   shapekit->set("material", "diffuseColor 1 0 0");
  \endcode
  
  ..while doing it without shape kits amounts to this:

  \code
  SoSeparator * root = new SoSeparator;
  SoMaterial * material = new SoMaterial;
  material->diffuseColor.setValue(1,0,0);
  root->addChild(material);
  root->addChild(new SoCube);
  \endcode

  ..so even for this miniscule mock-up example, you save on code
  verbosity and complexity.


  \TOOLMAKER_REF


  Following is a complete example of a node kit extension. The node
  kit is a kit which automatically scales a shape so it will be the
  same size in screen-pixels, no matter which distance it is from the
  camera. This is useful for marker graphics. The shape defaults to a
  cube, but can be set by the programmer to any shape or scene
  sub-graph.

  The header file:
  \code
  // Copyright (C) 1998-2002 by Systems in Motion. All rights reserved.

  #ifndef COIN_SHAPESCALE_H
  #define COIN_SHAPESCALE_H
  #include <Inventor/nodekits/SoSubKit.h>
  #include <Inventor/nodekits/SoBaseKit.h>
  #include <Inventor/fields/SoSFFloat.h>

  class SbViewport;
  class SoState;
  class SbColor;
  class SbVec2s;
  
  class ShapeScale : public SoBaseKit {
    typedef SoBaseKit inherited;
  
    SO_KIT_HEADER(ShapeScale);
    
    SO_KIT_CATALOG_ENTRY_HEADER(topSeparator);
    SO_KIT_CATALOG_ENTRY_HEADER(scale);
    SO_KIT_CATALOG_ENTRY_HEADER(shape);
    
   public:
    ShapeScale(void);

    static void initClass(void);

   public:

    SoSFFloat active;
    SoSFFloat projectedSize;

   protected:
    virtual void GLRender(SoGLRenderAction * action);
    virtual ~ShapeScale();
  
  };

  #endif // ! SHAPESCALE_H
  \endcode

  The source code for the example:
  \code
  // Copyright (C) 1998-2002 by Systems in Motion. All rights reserved.
  
  //  The ShapeScale class is used for scaling a shape based on
  //  projected size.
  //
  //  This nodekit can be inserted in your scene graph to add for
  //  instance 3D markers that will be of a constant projected size.
  //
  //  The marker shape is stored in the "shape" part. Any kind of node
  //  can be used, even group nodes with several shapes, but the
  //  marker shape should be approximately of unit size, and with a
  //  center position in (0, 0, 0).
  
  
  //  SoSFFloat ShapeScale::active
  //  Turns the scaling on/off. Default value is TRUE.
  

  //  SoSFFloat ShapeScale::projectedSize
  //  The requested projected size of the shape. Default value is 5.0.

  #include "ShapeScale.h"
  
  #include <Inventor/actions/SoGLRenderAction.h>
  #include <Inventor/nodes/SoShape.h>
  #include <Inventor/nodes/SoScale.h>
  #include <Inventor/nodes/SoCube.h>
  #include <Inventor/nodes/SoSeparator.h>
  #include <Inventor/elements/SoViewVolumeElement.h>
  #include <Inventor/elements/SoViewportRegionElement.h>
  #include <Inventor/elements/SoModelMatrixElement.h>
  
  SO_KIT_SOURCE(ShapeScale);
  
  
  //  Constructor.
  ShapeScale::ShapeScale(void) 
  {
    SO_KIT_CONSTRUCTOR(ShapeScale);
  
    SO_KIT_ADD_FIELD(active, (TRUE));
    SO_KIT_ADD_FIELD(projectedSize, (5.0f));  

    SO_KIT_ADD_CATALOG_ENTRY(topSeparator, SoSeparator, FALSE, this, \x0, FALSE);
    SO_KIT_ADD_CATALOG_ABSTRACT_ENTRY(shape, SoNode, SoCube, TRUE, topSeparator, \x0, TRUE);
    SO_KIT_ADD_CATALOG_ENTRY(scale, SoScale, FALSE, topSeparator, shape, FALSE);
    
    SO_KIT_INIT_INSTANCE();
  }
    
    
  // Destructor.
  ShapeScale::~ShapeScale()
  {
  }
    
  // Initializes this class. Call before using it.

  void
  ShapeScale::initClass(void)
  {
    SO_KIT_INIT_CLASS(ShapeScale, SoBaseKit, "BaseKit");
  }

  static void 
  update_scale(SoScale * scale, const SbVec3f & v)
  {
    // only write to field when scaling has changed.
    if (scale->scaleFactor.getValue() != v) {
      scale->scaleFactor = v;
    }
  }

  // Overridden to (re)initialize scaling before rendering marker.
  void 
  ShapeScale::GLRender(SoGLRenderAction * action)
  {
    SoState * state = action->getState();

    SoScale * scale = (SoScale*) this->getAnyPart(SbName("scale"), TRUE);
    if (!this->active.getValue()) {
      update_scale(scale, SbVec3f(1.0f, 1.0f, 1.0f));
    }
    else {
      const SbViewportRegion & vp = SoViewportRegionElement::get(state);
      const SbViewVolume & vv = SoViewVolumeElement::get(state);
      SbVec3f center(0.0f, 0.0f, 0.0f);
      float nsize = this->projectedSize.getValue() / float(vp.getViewportSizePixels()[1]);
      SoModelMatrixElement::get(state).multVecMatrix(center, center); // transform to WCS
      float scalefactor = vv.getWorldToScreenScale(center, nsize);
      update_scale(scale, SbVec3f(scalefactor, scalefactor, scalefactor));
    }
    inherited::GLRender(action);
  }
  \endcode

  And a complete example showing how one can use this node kit:
  \code
  // Copyright (C) 1998-2002 by Systems in Motion. All rights reserved.

  #include <Inventor/Qt/SoQt.h>
  #include <Inventor/Qt/viewers/SoQtExaminerViewer.h>
  #include <Inventor/SoInput.h>
  #include <Inventor/SoOutput.h>
  #include <Inventor/SoPickedPoint.h>
  #include <Inventor/actions/SoRayPickAction.h>
  #include <Inventor/events/SoMouseButtonEvent.h>
  #include <Inventor/nodes/SoBaseColor.h>
  #include <Inventor/nodes/SoCube.h>
  #include <Inventor/nodes/SoEventCallback.h>
  #include <Inventor/nodes/SoSeparator.h>
  #include <Inventor/nodes/SoSwitch.h>
  #include <Inventor/nodes/SoTranslation.h>
  #include <assert.h>
  #include <stdlib.h>
  #include <time.h>

  #include "ShapeScale.h"

  // Returns random value between 0.0f and 1.0f.
  static float
  normalized_rand(void)
  {
    return float(rand())/float(RAND_MAX);
  }

  static SoSeparator *
  construct_new_marker(const SbVec3f & v)
  {
    SoSeparator * markerroot = new SoSeparator;

    SoTranslation * t = new SoTranslation;
    t->translation = v;
    markerroot->addChild(t);
    
    ShapeScale * kit = new ShapeScale;
    kit->active = TRUE;
    kit->projectedSize = 5.0f;
    
    // create the marker
    SoSeparator * markersep = new SoSeparator;
    
    SoBaseColor * mat = new SoBaseColor;
    mat->rgb.setValue(normalized_rand(), normalized_rand(), normalized_rand());
    markersep->addChild(mat);
    
    // marker shape should be unit size, with center in (0.0f, 0.0f, 0.0f)
    SoCube * cube = new SoCube;
    cube->width = 1.0f;
    cube->height = 1.0f;
    cube->depth = 1.0f;
    
    markersep->addChild(cube);
    kit->setPart("shape", markersep);
    markerroot->addChild(kit);
    
    return markerroot;
  }

  static void
  event_cb(void * ud, SoEventCallback * n)
  {
    const SoMouseButtonEvent * mbe = (SoMouseButtonEvent *)n->getEvent();
  
    if (mbe->getButton() == SoMouseButtonEvent::BUTTON1 &&
      mbe->getState() == SoButtonEvent::DOWN) {

      SoQtExaminerViewer * viewer = (SoQtExaminerViewer *)ud;
    
      SoRayPickAction rp(viewer->getViewportRegion());
      rp.setPoint(mbe->getPosition());
      rp.apply(viewer->getSceneManager()->getSceneGraph());
      
      SoPickedPoint * point = rp.getPickedPoint();
      if (point == NULL) {
        (void)fprintf(stderr, "\n** miss! **\n\n");
        return;
      }

      n->setHandled();

      const SoPath * p = rp.getCurPath();

      for (int i = 0; i < p->getLength(); i++) {
        SoNode * n = p->getNodeFromTail(i);
        if (n->isOfType(SoGroup::getClassTypeId())) {
          SoGroup * g = (SoGroup *)n;
          g->addChild(construct_new_marker(point->getPoint()));
          break;
        }
      }
    }
  }

  void
  show_instructions( void )
  {
    (void)fprintf(stdout, 
      "\nThis example program demonstrates the use of the ShapeScale nodekit.\n"
      "\nQuick instructions:\n\n"
      "  * place the marker by clicking on a shape with the left mouse button\n"
      "  * hit ESC to toggle back and forth to view mode\n"
      "  * zoom back and forth to see how the markers stay the same size\n\n");
  }

  int
  main(int argc, char ** argv)
  {
    if (argc != 2) {
      (void) fprintf(stderr,"\nSpecify an Inventor file as argument.\n");
      return -1;
    }

    QWidget * window = SoQt::init( argv[0] );
    ShapeScale::initClass(); // init our extension nodekit

    SoQtExaminerViewer * ex1 = new SoQtExaminerViewer( window );
  
    SoInput input;
    SbBool ok = input.openFile(argv[1]);
    if (!ok) {
      (void) fprintf(stderr, "Unable to open file: %s\n", argv[1]);
      return -1;
    }

    SoSeparator * root = SoDB::readAll(&input); 
  
    if (root == NULL) {
      (void) fprintf(stderr, "Unable to read file: %s\n", argv[1]);
      return -1;
    }

    show_instructions();

    SoSeparator * newroot = new SoSeparator;
    newroot->ref();
  
    newroot->addChild(root);

    // create event callback and marker nodes
    SoSeparator * sep = new SoSeparator;
    newroot->addChild(sep);
  
    SoEventCallback * ecb = new SoEventCallback;
    ecb->addEventCallback(SoMouseButtonEvent::getClassTypeId(), event_cb, ex1);
    sep->addChild(ecb);
  
    ex1->setSceneGraph(newroot);
    ex1->setTransparencyType(SoGLRenderAction::SORTED_OBJECT_BLEND);
    ex1->setViewing(FALSE);
    
    ex1->show();
    SoQt::show( window );

    SoQt::mainLoop();
    delete ex1;

    newroot->unref();
    return 0;
  }
  \endcode
*/

#include <Inventor/nodekits/SoBaseKit.h>
#include <Inventor/nodekits/SoSubKitP.h>
#include <Inventor/nodekits/SoNodeKitListPart.h>
#include <Inventor/nodes/SoSeparator.h>
#include <Inventor/nodes/SoCallback.h>
#include <Inventor/nodes/SoEventCallback.h>
#include <Inventor/misc/SoChildList.h>
#include <Inventor/actions/SoSearchAction.h>
#include <Inventor/actions/SoGetMatrixAction.h>
#include <Inventor/actions/SoRayPickAction.h>
#include <Inventor/actions/SoGetBoundingBoxAction.h>
#include <Inventor/actions/SoWriteAction.h>
#include <Inventor/SoInput.h>
#include <Inventor/SoOutput.h>
#include <Inventor/details/SoNodeKitDetail.h>
#include <Inventor/SoPickedPoint.h>
#include <Inventor/lists/SoPickedPointList.h>
#include <stdlib.h>
#include <limits.h>
#include <ctype.h>
#include <string.h>
#include <../tidbits.h> // coin_isspace()
#include <coindefs.h> // COIN_OBSOLETED()
#include <Inventor/errors/SoDebugError.h>

#ifndef DOXYGEN_SKIP_THIS

class SoBaseKitP {
public:
  SoBaseKitP(SoBaseKit * kit) : kit(kit) { }
public:
  SoBaseKit * kit;
  SoFieldData * writedata;
  SbList<SoSFNode*> instancelist;
  SbBool didcount;

public:
  void addKitDetail(SoFullPath * path, SoPickedPoint * pp);
  void createWriteData(void);
  void testParentWrite(void);

  void copyParts(const SoBaseKit * srckit, SbList <SoNode*> & partlist,
                 const SbBool copyconnections);

  void setParts(SbList <SoNode*> partlist, const SbBool leafparts);
};

#endif // DOXYGEN_SKIP_THIS

SbBool SoBaseKit::searchchildren = FALSE;

SO_KIT_SOURCE(SoBaseKit);

/*!
  \fn const SoNodekitCatalog * SoBaseKit::getClassNodekitCatalog(void)
  Returns the nodekit catalog which defines the layout of this
  class' kit.
*/

/*!
  \fn const SoNodekitCatalog * SoBaseKit::getNodekitCatalog(void) const
  Returns the nodekit catalog which defines the layout of this
  class' kit.
*/

/*!
  \fn const SoNodekitCatalog ** SoBaseKit::getClassNodekitCatalogPtr(void)
  Returns the pointer to the pointer of the nodekit catalog
  for this class.
*/


/*!
  \var SoChildList * SoBaseKit::children
  \COININTERNAL
*/
/*!
  \var SbBool SoBaseKit::connectionsSetUp
  \COININTERNAL
*/

// macro for accessing private data
#define THIS this->pimpl

/*!
  Constructor.
*/
SoBaseKit::SoBaseKit(void)
{
  THIS = new SoBaseKitP(this);
  THIS->writedata = NULL;

  SO_KIT_INTERNAL_CONSTRUCTOR(SoBaseKit);

  // Can't use ADD_CATALOG_ENTRY macro for the toplevel "this" entry,
  // as we don't want to call SO_NODE_ADD_FIELD(). This is how the
  // invocation would have looked if we could use the macro:
  //
  // SO_KIT_ADD_CATALOG_ENTRY(this, SoBaseKit, TRUE, "", "", FALSE);

  if (SO_KIT_IS_FIRST_INSTANCE()) {
    SoBaseKit::classcatalog->addEntry("this",
                                      SoBaseKit::getClassTypeId(),
                                      SoBaseKit::getClassTypeId(),
                                      TRUE,
                                      "",
                                      "",
                                      FALSE,
                                      SoType::badType(),
                                      SoType::badType(),
                                      FALSE);
    
  }


  SO_KIT_ADD_CATALOG_LIST_ENTRY(callbackList, SoSeparator, TRUE, this, "", SoCallback, TRUE);
  SO_KIT_ADD_LIST_ITEM_TYPE(callbackList, SoEventCallback);

  // this could be created on demand, but will make it more complicated
  this->children = new SoChildList(this);

  this->connectionsSetUp = FALSE;
  SO_KIT_INIT_INSTANCE();
}

/*!
  Destructor.
*/
SoBaseKit::~SoBaseKit()
{
  delete this->children;
  delete THIS;
}

// Doc in superclass
void
SoBaseKit::initClass(void)
{
  SO_NODE_INTERNAL_INIT_CLASS(SoBaseKit, SO_FROM_INVENTOR_1);
  // set rayPick method
  SoType type = SoBaseKit::getClassTypeId();
  SoRayPickAction::addMethod(type, SoNode::rayPickS);
}

/*!
  Returns a pointer to the node part with \a partname.

  If the part is not in the nodekit's catalog, return \c NULL.

  If the part is in the catalog, has not been made and \a makeifneeded
  is \c TRUE, construct the part and all it's parents, and return the
  node pointer. If the node part has not been made and \a makeifneeded
  is \c FALSE, return \c NULL.

  FIXME: describe syntax for specifiying "paths". 19991205 mortene.
*/
SoNode *
SoBaseKit::getPart(const SbName & partname, SbBool makeifneeded)
{
  return this->getAnyPart(partname, makeifneeded, TRUE, TRUE);
}

/*!
  FIXME: write function documentation
*/
SbString
SoBaseKit::getPartString(const SoBase * part)
{
  const SoNodekitCatalog * catalog = this->getNodekitCatalog();
  if (part->isOfType(SoNode::getClassTypeId())) {
    int idx = this->findNodeInThisKit((SoNode *)part);
    if (idx >= 0) {
      return SbString(catalog->getName(idx).getString());
    }
    return SbString();
  }
  else if (part->isOfType(SoPath::getClassTypeId())) {
    SoFullPath * path = (SoFullPath *)part;
    int pathidx = path->findNode(this);
    if (pathidx < 0) return SbString();
    SoBaseKit * kit = this;
    SbString partname;
    int parentnum = 0;
    SoNode * tail = path->getTail();
    SoNode * node = kit;
    while (node != tail) {
      node = path->getNode(++pathidx);
      int partnum = kit->findNodeInThisKit(node, parentnum);
      if (partnum < 0) {
#if COIN_DEBUG
        SoDebugError::postWarning("SoBaseKit::getPartString",
                                  "Illegal path");
#endif // COIN_DEBUG
        return SbString();
      }
      if (catalog->isLeaf(partnum)) {
        if (partname != "") partname += '.';
        partname += catalog->getName(partnum).getString();
      }
      if (node->isOfType(SoNodeKitListPart::getClassTypeId())) {
        // no sense in using SoNodeKitListPart as a non-leaf node, right?
        assert(catalog->isLeaf(partnum));
        SoNodeKitListPart * list = (SoNodeKitListPart *)node;
        pathidx += 2; // // skip container node
        if (pathidx >= path->getLength()) {
#if COIN_DEBUG
          SoDebugError::postWarning("SoBaseKit::getPartString",
                                    "Path too short");
#endif // COIN_DEBUG
          return SbString();
        }
        node = path->getNode(pathidx);
        int childidx = list->findChild(node);
        assert(childidx >= 0);
        partname += '[';
        partname.addIntString(childidx);
        partname += ']';
      }
      if (node->isOfType(SoBaseKit::getClassTypeId())) {
        kit = (SoBaseKit *) node;
        catalog = kit->getNodekitCatalog();
        parentnum = 0;
      }
      else {
        // search more in this kit
        parentnum = partnum;
      }
    }
    return partname;
  }
  return SbString();
}

/*!
  FIXME: write function documentation
*/
SoNodeKitPath *
SoBaseKit::createPathToPart(const SbName & partname, SbBool makeifneeded, const SoPath * pathtoextend)
{
  return this->createPathToAnyPart(partname, makeifneeded, TRUE, TRUE, pathtoextend);
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setPart(const SbName & partname, SoNode * from)
{
  return this->setAnyPart(partname, from, FALSE);
}

static const char *
skip_spaces(const char * ptr)
{
  // ANSI C isspace() takes the current locale into account. Under
  // MSWindows, this can lead to "interesting" artifacts, like a case
  // with RR tracked down and fixed by <thammer@sim.no> where a
  // character (was it �?) with ASCII value > 127 made isspace()
  // return non-nil on a German system. So we're using our own
  // locale-independent isspace() implementation instead.
  while (coin_isspace(*ptr)) ptr++;
  return ptr;
}

static int
find_partname_length(const char * ptr)
{
  int cnt = 0;
  while (ptr[cnt] && !coin_isspace(ptr[cnt]) &&
         ptr[cnt] != '{' && ptr[cnt] != '}') {
    cnt++;
  }
  return cnt;
}

/*!

  Sets nodekit part field values. The input argument string is of the
  format:

  \code
  partname {
    fieldname fieldval
    fieldname fieldval
    [...]
  }
  partname {
    fieldname fieldval
    fieldname fieldval
    [...]
  }
  [...]
  \endcode

  (Whitespace layout is ignored, as always for Inventor format input
  strings.)

  Here's an example, changing several values of the camera part of an
  SoCameraKit instance:

  \code
  camera { heightAngle 0.3927  nearDistance 1.1  farDistance 999.9 }
  \endcode
*/
SbBool
SoBaseKit::set(const char * namevaluepairliststring)
{
  int stringlen = strlen(namevaluepairliststring); // cache this value
  const char * currptr = skip_spaces(namevaluepairliststring);
  SoInput memInput;

  while (*currptr) {
    int partnamelen = find_partname_length(currptr);
    const char * start = skip_spaces(currptr + partnamelen);
    if (*start != '{') { // first non-space after partname should be a {
#if COIN_DEBUG
      SoDebugError::postWarning("SoBaseKit::set",
                                "parse error at byte %d in input string",
                                start-namevaluepairliststring);
#endif // COIN_DEBUG
      return FALSE;
    }
    start++; // skip {
    SbString partname(currptr, 0, partnamelen-1);
    SoBaseKit * kit = this;
    int partNum;
    SbBool isList;
    int listIdx;
    if (!SoBaseKit::findPart(partname, kit, partNum, isList, listIdx, TRUE, NULL, TRUE)) {
#if COIN_DEBUG
      SoDebugError::postWarning("SoBaseKit::set",
                                "part ``%s'' not found",
                                partname.getString());
#endif // COIN_DEBUG
      return FALSE;
    }

    SoNode * node = kit->pimpl->instancelist[partNum]->getValue();
    kit->pimpl->instancelist[partNum]->setDefault(FALSE);

    if (isList) {
      SoNodeKitListPart * list = (SoNodeKitListPart *)node;
      if (listIdx < 0 || listIdx > list->getNumChildren()) {
#if COIN_DEBUG
        SoDebugError::postWarning("SoBaseKit::set",
                                  "index %d out of bounds for part ``%s''",
                                  listIdx, partname.getString());
#endif // COIN_DEBUG
        return FALSE;
      }
      else if (listIdx == list->getNumChildren()) {
        if (!list->canCreateDefaultChild()) {
#if COIN_DEBUG
          SoDebugError::postWarning("SoBaseKit::set",
                                    "Unable to create default child for list-part ``%s''",
                                    partname.getString());
#endif // COIN_DEBUG
          return FALSE;
        }
        node = list->createAndAddDefaultChild();
      }
      else {
        node = list->getChild(listIdx);
      }
    }
    memInput.setBuffer((void *)start, stringlen - (start-namevaluepairliststring));
    SbBool dummy;
    if (!node->getFieldData()->read(&memInput, node, TRUE, dummy)) {
#if COIN_DEBUG
      SoDebugError::postWarning("SoBaseKit::set",
                                "error while parsing data for part ``%s''",
                                partname.getString());
#endif // COIN_DEBUG
      return FALSE;
    }
    currptr = start + (int) memInput.getNumBytesRead();
    if (*currptr == '}') currptr++;
    assert(currptr <= namevaluepairliststring + stringlen);
    currptr = skip_spaces(currptr);
  }
  return TRUE;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::set(const char * partnamestring, const char * parameterstring)
{
  SbString partname(partnamestring);
  int partNum;
  SbBool isList;
  int listIdx;
  SoBaseKit * kit = this;
  if (SoBaseKit::findPart(partname, kit, partNum, isList, listIdx, TRUE, NULL, TRUE)) {
    SoNode * node = kit->pimpl->instancelist[partNum]->getValue();
    kit->pimpl->instancelist[partNum]->setDefault(FALSE);
    assert(node != NULL); // makeifneeded was TRUE in findPart call
    if (isList) {
      assert(node->isOfType(SoNodeKitListPart::getClassTypeId()));
      SoNodeKitListPart * list = (SoNodeKitListPart *) node;
      if (listIdx < 0 || listIdx > list->getNumChildren()) {
#if COIN_DEBUG
        SoDebugError::postWarning("SoBaseKit::set",
                                  "index %d out of bounds for part ``%s''",
                                  listIdx, partnamestring);
#endif // COIN_DEBUG
        return FALSE;
      }
      else if (listIdx == list->getNumChildren()) {
        if (!list->canCreateDefaultChild()) {
#if COIN_DEBUG
          SoDebugError::postWarning("SoBaseKit::set",
                                    "Unable to create default child for list-part ``%s''",
                                    partname.getString());
#endif // COIN_DEBUG
          return FALSE;
        }
        node = list->createAndAddDefaultChild();
      }
      else {
        node = list->getChild(listIdx);
      }
    }
    if (node) {
      SoInput memInput;
      SbBool dummy;
      memInput.setBuffer((void *)parameterstring, strlen(parameterstring));
      const SoFieldData * fielddata = node->getFieldData();
      return fielddata->read(&memInput, node, TRUE, dummy);
    }
  }
  return FALSE;
}

// Doc in superclass.
void
SoBaseKit::doAction(SoAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->children->traverseInPath(action, numindices, indices);
  }
  else {
    this->children->traverse(action);
  }
}

// Doc in superclass.
void
SoBaseKit::callback(SoCallbackAction * action)
{
  SoBaseKit::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoBaseKit::GLRender(SoGLRenderAction * action)
{
  SoBaseKit::doAction((SoAction *)action);
}

// Doc in superclass. Overriden to calculate bounding box center.
void
SoBaseKit::getBoundingBox(SoGetBoundingBoxAction * action)
{
  int numindices;
  const int * indices;
  int last = action->getPathCode(numindices, indices) == SoAction::IN_PATH ?
    indices[numindices-1] : this->children->getLength() - 1;

  SbVec3f acccenter(0.0f, 0.0f, 0.0f);
  int numacc = 0;
  
  for (int i = 0; i <= last; i++) {
    this->children->traverse(action, i, i);
    if (action->isCenterSet()) {
      acccenter += action->getCenter();
      numacc++;
      action->resetCenter();
    }
  }
  if (numacc) action->setCenter(acccenter / float(numacc), FALSE);
}

// Doc in superclass.
void
SoBaseKit::getMatrix(SoGetMatrixAction * action)
{
  int numindices;
  const int * indices;
  if (action->getPathCode(numindices, indices) == SoAction::IN_PATH) {
    this->children->traverseInPath(action, numindices, indices);
  }
}

// Doc in superclass.
void
SoBaseKit::handleEvent(SoHandleEventAction * action)
{
  SoBaseKit::doAction((SoAction *)action);
}

// Doc in superclass.
void
SoBaseKit::rayPick(SoRayPickAction * action)
{
  SoBaseKit::doAction((SoAction *)action);

  const SoPickedPointList & pplist = action->getPickedPointList();
  const int n = pplist.getLength();
  for (int i = 0; i < n; i++) {
    SoPickedPoint * pp = pplist[i];
    SoFullPath * path = (SoFullPath*) pp->getPath();
    if (path->containsNode(this) && pp->getDetail(this) == NULL) {
      THIS->addKitDetail(path, pp);
    }
  }
}

// Doc in superclass.
void
SoBaseKit::search(SoSearchAction * action)
{
  inherited::search(action);
  if (action->isFound() || !SoBaseKit::searchchildren) return;
  SoBaseKit::doAction((SoAction *)action);
}

// Test if node has all fields set to default and if the fields
// contains the default values. If so, we don't need to write it.
static SbBool
is_default_node(SoNode * node, const SoType & typecheck)
{
  if (node->getTypeId() != typecheck) return FALSE;
  if (node->getChildren() && node->getChildren()->getLength()) return FALSE;

  SoNode * definstance = NULL;
  const SoFieldData * fielddata = node->getFieldData();
  int i, n = fielddata->getNumFields();
  for (i = 0; i < n; i++) {
    SoField * field = fielddata->getField(node, i);
    if (!field->isDefault()) break;
    if (field->isConnectionEnabled() && field->isConnected()) break;
    if (definstance == NULL) {
      definstance = (SoNode*) typecheck.createInstance();
      definstance->ref();
    }
    if (!field->isSame(*fielddata->getField(definstance, i))) break;
  }
  if (definstance) definstance->unref();
  // if all fields were tested, it is a default node.
  return i == n;
}

// Doc in superclass.
void
SoBaseKit::write(SoWriteAction * action)
{
  SoOutput * out = action->getOutput();
  if (out->getStage() == SoOutput::COUNT_REFS) {
    this->addWriteReference(out, FALSE);
  }
  else if (out->getStage() == SoOutput::WRITE) {
    if (this->writeHeader(out, FALSE, FALSE)) return; // no more to write
    // if THIS->writedata != NULL, we must found that some parts or
    // fields need to be written.
    if (THIS->writedata) {
      const SoNodekitCatalog * catalog = this->getNodekitCatalog();

      // loop through parts and see if we need to call setDefault(FALSE)
      // on some of the fields.
      int n = THIS->instancelist.getLength();
      for (int i = 1; i < n; i++) {
        SoSFNode * field = THIS->instancelist[i];
        if (field->isDefault()) {
          SoNode * node = field->getValue();
          if (node) {
            if (node->shouldWrite()) {
              field->setDefault(FALSE);
            }
            else if (node->isOfType(SoBaseKit::getClassTypeId())) {
              SoBaseKit * kit = (SoBaseKit*) node;
              if (kit->forceChildDrivenWriteRefs(out)) {
                field->setDefault(FALSE);
              }
            }
          }
        }
      }

      // use THIS->writedata to get correct order of fields
      THIS->writedata->write(out, this);

      // we don't need it any more
      delete THIS->writedata;
      THIS->writedata = NULL;
    }
    this->writeFooter(out);
  }
  else assert(0 && "unknown stage");
}

// documented in superclass
void
SoBaseKit::addWriteReference(SoOutput * out, SbBool isfromfield)
{
  // don't call inherited::addWriteReference(), as we will handle
  // the fields ourselves, using a new fielddata. This is needed to
  // write fields in the correct order.
  SoBase::addWriteReference(out, isfromfield);

  // If first invocation during the reference counting pass, check
  // nodes in our catalog.
  if (!isfromfield && !this->hasMultipleWriteRefs()) {
    this->countMyFields(out);
  }
}

/*!
  Reference count connections to nodes in the catalog.
*/
void
SoBaseKit::countMyFields(SoOutput * out)
{
  if (out->getStage() != SoOutput::COUNT_REFS)
    return;

  // already created?
  if (THIS->writedata) return;

  // create THIS->writedata, which contains a sorted list of fields.
  THIS->createWriteData();

  const SoNodekitCatalog * catalog = this->getNodekitCatalog();

  // test if some fields that are default should write anyway
  int i, n = THIS->instancelist.getLength();
  for (i = 1; i < n; i++) {
    SoSFNode * field = THIS->instancelist[i];
    if (field->isDefault()) {
      SoNode * node = field->getValue();
      if (node == NULL && ! catalog->isNullByDefault(i)) {
        field->setDefault(FALSE);
      }
    }
  }

  // sets fields that should not be written to default, this
  // is a virtual methods, so subkits can do some work when needed.
  this->setDefaultOnNonWritingFields();

  // test if parent of parts is writing. Then we must write part anyway.
  THIS->testParentWrite();


  n = THIS->writedata->getNumFields();
  for (i = 0; i < n; i++) {
    const SbName name = THIS->writedata->getFieldName(i);
    SoField * field = THIS->writedata->getField(this, i);
    int partnum = catalog->getPartNumber(name);
    if (partnum < 0) {
      // field is not a part. Do normal field write.
      if (field->shouldWrite()) {
        field->write(out, name);
      }
    }
    else {
      if (!field->isDefault()) field->write(out, name);
      else {
        SoSFNode * part = (SoSFNode*) field;
        // don't use SoSFNode::countWriteRefs() as we are trying
        // to avoid writing this field/part.
        part->SoField::countWriteRefs(out);
        SoNode * node = part->getValue();
        if (node && node->isOfType(SoBaseKit::getClassTypeId())) {
          // recurse
          ((SoBaseKit*)node)->countMyFields(out);
        }
      }
    }
  }
}

// Note: the following documentation for
// setDefaultOnNonWritingFields() will also be used for nodekit and
// dragger subclasses, so keep it general.
/*!
  (Be aware that this method is unlikely to be of interest to the
  application programmer who does not want to extend the library with
  new custom nodekits or draggers.  If you indeed \e are writing
  extensions, see the information in the SoBaseKit class
  documentation.)

  A virtual method that should call SoField::setDefault() with
  argument \c TRUE on part fields that should not be written upon
  scenegraph export operations.

  This is typically done when:

  <OL>

  <LI> field value is \c NULL and part is \c NULL by default </LI>

  <LI> it is a leaf SoGroup or SoSeparator node with no children </LI>

  <LI> it is a leaf listpart with no children and a SoGroup or
  SoSeparator container </LI>

  <LI> it is a non-leaf part and it's of SoGroup type and all fields
  are at their default values </LI>

  </OL>

  Subclasses should usually override this to do additional settings
  for new member fields.  From the subclass, do remember to call
  "upwards" to your superclass' setDefaultOnNonWritingFields() method.
*/
void
SoBaseKit::setDefaultOnNonWritingFields(void)
{
  const SoNodekitCatalog * catalog = this->getNodekitCatalog();
  int n = THIS->instancelist.getLength();
  for (int i = 1; i < n; i++) {
    SoSFNode * field = THIS->instancelist[i];
    if (!field->isDefault()) {
      SoNode * node = field->getValue();
      // first test
      if (node == NULL && catalog->isNullByDefault(i)) {
        field->setDefault(TRUE);
      }
      else if (node) {
        SbBool leaf = catalog->isLeaf(i);
        SoType type = node->getTypeId();

        if (leaf) {
          // second test
          if ((type == SoGroup::getClassTypeId() ||
               type == SoSeparator::getClassTypeId()) &&
              ((SoGroup*)node)->getNumChildren() == 0) {
            field->setDefault(TRUE);
          }
          // third test
          else if (type == SoNodeKitListPart::getClassTypeId()) {
            SoNodeKitListPart * list = (SoNodeKitListPart*) node;
            SoNode * container = list->getContainerNode();
            if (list->getNumChildren() == 0 && container &&
                (container->getTypeId() == SoSeparator::getClassTypeId() ||
                 container->getTypeId() == SoGroup::getClassTypeId())) {
              field->setDefault(TRUE);
            }
          }
        }
        else { // not leaf
          // fourth test
          if (node->isOfType(SoGroup::getClassTypeId()) &&
              is_default_node(node, node->getTypeId())) {
            field->setDefault(TRUE);
          }
        }
      }
    }
  }
}

/*!
  Returns \c TRUE if kit should write. This happens if shouldWrite()
  returns \c TRUE, or if any of the children (recursively) should
  write.
*/
SbBool
SoBaseKit::forceChildDrivenWriteRefs(SoOutput * out)
{
  if (this->shouldWrite()) return TRUE;

  // if NULL we already did this test, found that we shouldn't write,
  // deleted writedata and set writedata to NULL.
  if (!THIS->writedata) return FALSE;

  const SoNodekitCatalog * catalog = this->getNodekitCatalog();
  int i, n = THIS->writedata->getNumFields();

  // loop through fields and break as soon as we find a reason
  // to write
  for (i = 0; i < n; i++) {
    SoField * field = THIS->writedata->getField(this, i);
    int partnum = catalog->getPartNumber(THIS->writedata->getFieldName(i));
    if (!field->isDefault()) break;
    else if (partnum < 0 && field->isIgnored()) break;
    else if (partnum > 0) {
      SoSFNode * part = (SoSFNode*) field;
      SoNode * node = part->getValue();
      if (node) {
        if (node->shouldWrite()) break;
        else if (node->isOfType(SoBaseKit::getClassTypeId())) {
          SoBaseKit * kit = (SoBaseKit*) node;
          // recurse
          if (kit->forceChildDrivenWriteRefs(out)) break;
        }
      }
    }
  }

  if (i < n) { // did we find a reason to write?
    SoBase::addWriteReference(out);
    return TRUE;
  }
  else {
    delete THIS->writedata;
    THIS->writedata = NULL;
    return FALSE;
  }
}


// Documented in superclass.
void
SoBaseKit::getPrimitiveCount(SoGetPrimitiveCountAction * action)
{
  SoBaseKit::doAction((SoAction *)action);
}

// Documented in superclass.
SoChildList *
SoBaseKit::getChildren(void) const
{
  return this->children;
}

/*!
  Print out the full nodekit catalog structure.  Just invokes
  SoBaseKit::printSubDiagram() on the catalog root. Useful for
  debugging.

  Example output:

  \verbatim
  CLASS SoWrapperKit
  -->"this"
        "callbackList"
        "topSeparator"
           "pickStyle"
           "appearance"
           "units"
           "transform"
           "texture2Transform"
           "childList"
  -->      "localTransform"
  -->      "contents"
  \endverbatim

  The arrows denote new entries in the catalog for the particular
  class versus it's superclass. (Apart from the root entry, of
  course.)

  For a more detailed catalog dump, see SoBaseKit::printTable().
*/
void
SoBaseKit::printDiagram(void)
{
  fprintf(stdout, "CLASS So%s\n", this->getTypeId().getName().getString());
  this->printSubDiagram("this", 0);
}

/*!
  Print out the nodekit catalog structure from \a rootname and
  downwards in the catalog tree, with indentation starting at \a
  level.

  \sa printDiagram()
*/
void
SoBaseKit::printSubDiagram(const SbName & rootname, int level)
{
  const SoNodekitCatalog * parentcatalog = NULL;
  if (this->getTypeId() != SoBaseKit::getClassTypeId()) {
    SoType parenttype = this->getTypeId().getParent();
    SoBaseKit * parentobj = (SoBaseKit *)parenttype.createInstance();
    parentcatalog = parentobj->getNodekitCatalog();
    parentobj->ref();
    parentobj->unref();
  }

  const SoNodekitCatalog * thiscat = this->getNodekitCatalog();

  int i = 0;
  if (!parentcatalog ||
      parentcatalog->getPartNumber(rootname) == SO_CATALOG_NAME_NOT_FOUND ||
      parentcatalog->getType(rootname) != thiscat->getType(rootname)) {
    fprintf(stdout, "-->");
    i++;
  }
  for (; i < level+1; i++) fprintf(stdout, "   ");

  fprintf(stdout, "\"%s\"\n", rootname.getString());

  for (int j=0; j < thiscat->getNumEntries(); j++) {
    if (thiscat->getParentName(j) == rootname)
      this->printSubDiagram(thiscat->getName(j), level + 1);
  }
}

/*!
  Write the complete nodekit catalog in table form.

  Example output:

  \verbatim
  CLASS SoWrapperKit
  PVT   "this",  SoWrapperKit  --- 
        "callbackList",  SoNodeKitListPart [ SoCallback, SoEventCallback ] 
  PVT   "topSeparator",  SoSeparator  --- 
        "pickStyle",  SoPickStyle  --- 
        "appearance",  SoAppearanceKit  --- 
        "units",  SoUnits  --- 
        "transform",  SoTransform  --- 
        "texture2Transform",  SoTexture2Transform  --- 
        "childList",  SoNodeKitListPart [ SoShapeKit, SoSeparatorKit ] 
        "localTransform",  SoTransform  --- 
        "contents",  SoSeparator  --- 
  \endverbatim

  \c PVT denotes that it's a private entry in the catalog, then
  follows the part name and the part type. If the part is a list, the
  allowed node types for the list is given in square brackets, and if
  not there's a triple hyphen. If the part type is abstract, the
  default part type will be listed last (not shown in the example
  output above).
*/
void
SoBaseKit::printTable(void)
{
  fprintf(stdout, "CLASS So%s\n", this->getTypeId().getName().getString());

  const SoNodekitCatalog * thiscat = this->getNodekitCatalog();
  for (int i=0; i < thiscat->getNumEntries(); i++) {
    const SoType t = thiscat->getType(i);
    fprintf(stdout, "%s   \"%s\",  So%s ",
            thiscat->isPublic(i) ? "   " : "PVT",
            thiscat->getName(i).getString(),
            t.getName().getString());
    if (thiscat->isList(i)) {
      SoTypeList tlist = thiscat->getListItemTypes(i);
      fprintf(stdout, "[ ");
      for (int j=0; j < tlist.getLength(); j++) {
        if (j) fprintf(stdout, ", ");
        fprintf(stdout, "So%s", tlist[j].getName().getString());
      }
      fprintf(stdout, " ] ");
    }
    else {
      fprintf(stdout, " --- ");
    }

    if (t != thiscat->getDefaultType(i)) {
      fprintf(stdout, ", (default type = So%s)",
              thiscat->getDefaultType(i).getName().getString());
    }
    fprintf(stdout, "\n");
  }
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::isSearchingChildren(void)
{
  return SoBaseKit::searchchildren;
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::setSearchingChildren(const SbBool newval)
{
  SoBaseKit::searchchildren = newval;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::typeCheck(const SbName & /*partname*/, const SoType & parttype, SoNode * node)
{
  if (node == NULL) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoBaseKit::typeCheck",
                           "node was NULL");
#endif // COIN_DEBUG
    return NULL;
  }
  if (!node->isOfType(parttype)) {
#if COIN_DEBUG
    SoDebugError::postInfo("SoBaseKit::typeCheck",
                           "wrong type: %s", node->getTypeId().getName().getString());
#endif // COIN_DEBUG
    return NULL;
  }
  return node;
}

// Documented in superclass. Overridden to also recurse on non-null
// part nodes.
SoNode *
SoBaseKit::addToCopyDict(void) const
{
  SoNode * cp = (SoNode*) SoFieldContainer::checkCopy(this);
  if (cp == NULL) { // not copied?
    cp = (SoNode*) this->getTypeId().createInstance();
    assert(cp);
    cp->ref();
    SoFieldContainer::addCopy(this, cp);
    cp->unrefNoDelete();

    int n = THIS->instancelist.getLength();
    for (int i = 1; i < n; i++) {
      SoNode * node = THIS->instancelist[i]->getValue();
      if (node != NULL) node->addToCopyDict();
    }
  }
  return cp;
}

// (Doc in superclass.) Overridden to copy parts correctly.
void
SoBaseKit::copyContents(const SoFieldContainer * fromfc,
                        SbBool copyconnections)
{
  int i;

  // disable connections while copying
  SbBool oldsetup = this->setUpConnections(FALSE);

  // do normal node copy
  inherited::copyContents(fromfc, copyconnections);

  const SoBaseKit * srckit = (const SoBaseKit*) fromfc;

  // convenient reference
  const SbList <SoSFNode*> & srcfields = srckit->getCatalogInstances();

  const int n = THIS->instancelist.getLength();

  // use temporary lists to store part node pointers and field
  // default flag, as we will modify the originals.
  SbList <SoNode *> partlist;
  SbList <SbBool> flaglist;

  // part 0 is this
  partlist.append(NULL);
  flaglist.append(FALSE);

  // initialize temporary lists
  for (i = 1; i < n; i++) {
    partlist.append(NULL);
    flaglist.append(THIS->instancelist[i]->isDefault());
  }

  // copy parts, taking care of scene graph
  THIS->copyParts(srckit, partlist, copyconnections);

  // remove all old children before setting parts again
  this->getChildren()->truncate(0);

  // reset part fields
  for (i = 1; i < n; i++) {
    THIS->instancelist[i]->setValue(NULL);
    THIS->instancelist[i]->setDefault(TRUE);
  }

  // set non-leaf nodes first
  THIS->setParts(partlist, FALSE);

  // then leaf nodes
  THIS->setParts(partlist, TRUE);

  // do final pass
  for (i = 1; i < n; i++) {
    // restore default flag for fields
    THIS->instancelist[i]->setDefault(flaglist[i]);

    // unref nodes in temporary list as they were ref'ed
    // when inserted
    if (partlist[i]) partlist[i]->unref();
  }

  // enable connections
  if (oldsetup) this->setUpConnections(TRUE);
}

/*!
  FIXME: write function documentation
*/
SoGroup *
SoBaseKit::getContainerNode(const SbName & listname, SbBool makeifneeded)
{
  SoBaseKit * kit = this;
  int partNum;
  SbBool isList;
  int listIdx;
  if (SoBaseKit::findPart(SbString(listname.getString()), kit, partNum,
                          isList, listIdx, makeifneeded, NULL, TRUE)) {
    SoNode * node = kit->pimpl->instancelist[partNum]->getValue();
    if (node == NULL) return NULL;
    assert(node->isOfType(SoNodeKitListPart::getClassTypeId()));
    SoNodeKitListPart * list = (SoNodeKitListPart *)node;
    return list->getContainerNode();
  }
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SoNode *
SoBaseKit::getAnyPart(const SbName & partname, SbBool makeifneeded, SbBool leafcheck, SbBool publiccheck)
{

  SoBaseKit * kit = this;
  int partNum;
  SbBool isList;
  int listIdx;

  SbString partstring(partname.getString());

  if (SoBaseKit::findPart(partstring, kit, partNum, isList, listIdx,
                          makeifneeded, NULL, TRUE)) {
    if (!publiccheck || kit->getNodekitCatalog()->isPublic(partNum)) {
      if (!leafcheck || kit->getNodekitCatalog()->isLeaf(partNum)) {
        if (isList) {
          SoNode * partnode = kit->pimpl->instancelist[partNum]->getValue();
          if (partnode == NULL) return NULL;
          assert(partnode->isOfType(SoNodeKitListPart::getClassTypeId()));
          SoNodeKitListPart * list = (SoNodeKitListPart *) partnode;
          if (listIdx >= 0 && listIdx < list->getNumChildren()) {
            return list->getChild(listIdx);
          }
          else if (makeifneeded && (listIdx == list->getNumChildren())) {
            if (!list->canCreateDefaultChild()) {
#if COIN_DEBUG
              SoDebugError::postWarning("SoBaseKit::getAnyPart",
                                        "Unable to create default child for list-part ``%s''",
                                        partname.getString());
#endif // COIN_DEBUG
            }
            return list->createAndAddDefaultChild();
          }
          else {
#if COIN_DEBUG
            SoDebugError::postWarning("SoBaseKit::getAnyPart",
                                      "index %d out of bounds for part ``%s''",
                                      listIdx, partname.getString());
#endif // COIN_DEBUG
          }
        }
        else {
          return kit->pimpl->instancelist[partNum]->getValue();
        }
      }
    }
  }
  // FIXME:
  // run cleanup?, in case some node has been temporarily created while
  // searching for the part?? pederb, 2000-01-05

#if COIN_DEBUG
  if (makeifneeded) { // user probably expected part to be found, post a warning
    SoDebugError::postWarning("SoBaseKit::getAnyPart",
                              "part ``%s'' not found in %s",
                              partname.getString(),
                              this->getTypeId().getName().getString());
  }
#endif // COIN_DEBUG
  return NULL;
}

/*!
  Return path with nested SoNodeKit instances down in the catalog
  hierarchy given by \a partname.

  If the trailing part has not been made and \a makeifneeded is \c
  TRUE, make an instance of the part type and insert into the catalog,
  as done in setAnyPart().

  If \a leafcheck is \c TRUE, ignore non-leaf catalog node entries. If
  \a publiccheck is \c TRUE, ignore private catalog entries.

  \a pathtoextend is a path through the nodekit instance catalog
  hierarchy, where we should pick up and continue to create the path
  from where \a pathtoextend terminates. If \a pathtoextend is \c
  NULL, we simply start at the "this" toplevel node.

  Returns \c NULL on failure, for any of the possible reasons
  described above (part ends in non-leaf or private catalog entry,
  part is not syntactically valid or refers to non-existing catalog
  entries).
*/
SoNodeKitPath *
SoBaseKit::createPathToAnyPart(const SbName & partname, SbBool makeifneeded,
                               SbBool leafcheck, SbBool publiccheck,
                               const SoPath * pathtoextend)
{
  SoFullPath * path;
  if (pathtoextend) {
    path = (SoFullPath *)pathtoextend->copy();
    // pop off nodes beyond this kit node
    if (path->containsNode(this)) while (path->getTail() != this && path->getLength()) path->pop();
    else if (path->getLength()) {
      SoNode * node = path->getTail();
      if (!node->getChildren() || node->getChildren()->find(this) < 0) {
#if COIN_DEBUG
        SoDebugError::postWarning("SoBaseKit::createPathToAnyPart",
                                  "pathtoextend is illegal");
#endif // COIN_DEBUG
        return NULL;
      }
      path->append(this); // this should be safe now
    }
  }
  else {
    path = (SoFullPath *)new SoPath(this);
  }
  path->ref();

  SoBaseKit * kit = this;
  int partNum;
  SbBool isList;
  int listIdx;

  if (SoBaseKit::findPart(SbString(partname.getString()), kit, partNum,
                          isList, listIdx, makeifneeded, path)) {
    const SoNodekitCatalog * catalog = kit->getNodekitCatalog();
    if ((leafcheck && ! catalog->isLeaf(partNum)) ||
        (publiccheck && ! catalog->isPublic(partNum))) {
      path->unref();
      return NULL;
    }

    SoNode * node = kit->pimpl->instancelist[partNum]->getValue();
    if (node) {
      path->append(node);
      if (isList) {
        SoNodeKitListPart * list = (SoNodeKitListPart *)node;
        int numlistchildren = list->getNumChildren();
        if (listIdx < 0 || listIdx > numlistchildren || (!makeifneeded && listIdx == numlistchildren)) {
#if COIN_DEBUG
          SoDebugError::postWarning("SoBaseKit::createPathToAnyPart",
                                    "index %d out of bounds for part ``%s''",
                                    listIdx, partname.getString());
#endif // COIN_DEBUG
          path->unref();
          return NULL;
        }
        else if (listIdx == numlistchildren) {
          if (!list->canCreateDefaultChild()) {
#if COIN_DEBUG
            SoDebugError::postWarning("SoBaseKit::createPathToAnyPart",
                                      "Unable to create default child for list-part ``%s''",
                                      partname.getString());
#endif //COIN_DEBUG

          }
          else {
            SoNode * newnode = list->createAndAddDefaultChild();
            path->append(list->getContainerNode());
            path->append(newnode);
          }
        }
        else {
          path->append(list->getContainerNode());
          path->append(list->getChild(listIdx));
        }
      }
      path->unrefNoDelete();
      return (SoNodeKitPath *)path;
    }
  }
  path->unref();
  return NULL;
}

/*!
  FIXME: write function documentation
*/
SbBool
SoBaseKit::setAnyPart(const SbName & partname, SoNode * from, SbBool anypart)
{
  SoBaseKit * kit = this;
  int partNum;
  SbBool isList;
  int listIdx;

  SbString partstring(partname.getString());

  if (SoBaseKit::findPart(partstring, kit, partNum, isList, listIdx, TRUE, NULL, TRUE)) {
    if (anypart || kit->getNodekitCatalog()->isPublic(partNum)) {
      if (isList) {
        SoNode * partnode = kit->pimpl->instancelist[partNum]->getValue();
        if (partnode) {
          assert(partnode->isOfType(SoNodeKitListPart::getClassTypeId()));
          SoNodeKitListPart * list = (SoNodeKitListPart *) partnode;
          if (listIdx >= 0 && listIdx <= list->getNumChildren()) {
            if (listIdx == list->getNumChildren())
              list->addChild(from);
            else
              list->replaceChild(listIdx, from);
            return TRUE;
          }
          else {
#if COIN_DEBUG
            SoDebugError::postWarning("SoBaseKit::setAnyPart",
                                      "index %d out of bounds for part ``%s''",
                                      listIdx, partname.getString());
#endif // COIN_DEBUG
          }
        }
      }
      else {
        return kit->setPart(partNum, from);
      }
    }
  }
  // FIXME:
  // run cleanup, in case some node has been temporarily created while
  // searching for the part?? pederb, 2000-01-05
  return FALSE;
}

/*!
  Not part of the Coin API.

  It is supposed to create the SoNodekitParts class instance. Since
  this class can only be used by SoBaseKit (all members are private,
  with SoBaseKit as friend), we decided to not support this class, and
  solve the problem of recording which parts are created in an
  alternative way.
*/
void
SoBaseKit::createNodekitPartsList(void)
{
  assert(0 &&
         "SoBaseKit::createNodekitPartsList() should not be used with Coin");
}

/*!
  Replaces the createNodekitPartsList() method.

  Sets up the list of SoSFNode fields with node pointers to the
  instances in our catalog.
*/
void
SoBaseKit::createFieldList(void)
{
  // FIXME:
  // is there any way to make sure this code is only run once, and in
  // the top level constructor. pederb, 2000-01-06
  //
  const SoNodekitCatalog * catalog = this->getNodekitCatalog();
  // only do this if the catalog has been created
  if (catalog) {
    THIS->instancelist.truncate(0);
    THIS->instancelist.append(NULL); // first catalog entry is "this"
    for (int i = 1; i < catalog->getNumEntries(); i++) {
      THIS->instancelist.append((SoSFNode *)this->getField(catalog->getName(i)));
      assert(THIS->instancelist[i] != NULL);
    }
  }
}

/*!
  FIXME: write function documentation
*/
void
SoBaseKit::createDefaultParts(void)
{
  // FIXME:
  // is there any way to make sure this code is only run once, and in
  // the top level constructor. pederb, 2000-01-06
  //
  const SoNodekitCatalog * catalog = this->getNodekitCatalog();
  // only do this if the catalog has been created
  if (catalog) {
    for (int i = 1; i < THIS->instancelist.getLength(); i++) {
      if (THIS->instancelist[i]->getValue() == NULL && !catalog->isNullByDefault(i)) {
        this->makePart(i);
        THIS->instancelist[i]->setDefault(TRUE);
      }
    }
  }
}

/*!
  In Open Inventor, this method returns a pointer to a private class.
  It will always return \c NULL in Coin.

  \sa createNodekitPartsList()
*/
const SoNodekitParts *
SoBaseKit::getNodekitPartsList(void) const
{
  assert(0 &&
         "SoBaseKit::getNodekitPartsList() obsoleted in Coin");
  return NULL;
}

/*!
  \COININTERNAL
*/
const SbList<SoSFNode*> &
SoBaseKit::getCatalogInstances(void) const
{
//    return this->fieldList;
  return THIS->instancelist;
}

/*!
  Obsoleted from the API in Coin.
*/
void
SoBaseKit::catalogError(void)
{
  COIN_OBSOLETED();
}

// Note: the following documentation for setUpConnections() will also
// be visible for subclass nodekits and draggers, so keep it general.
/*!
  Sets up all internal connections for instances of this class.

  (This method will usually not be of interest to the application
  programmer, unless you want to extend the library with new custom
  nodekits or dragger classes.  If so, see the SoBaseKit class
  documentation.)
*/
SbBool
SoBaseKit::setUpConnections(SbBool onoff, SbBool doitalways)
{
  return this->connectionsSetUp;
}

// doc in super
SbBool
SoBaseKit::readInstance(SoInput * in, unsigned short flags)
{
  int i;

  SbBool oldnotify = this->enableNotify(FALSE);
  SbBool oldsetup = this->setUpConnections(FALSE);

  // store old part values to find which parts are read
  SbList<SoNode*> nodelist;

  // Dummy first element to get indices to match instancelist (where
  // the dummy "this" catalog entry is first).
  nodelist.append(NULL);

  for (i = 1; i < THIS->instancelist.getLength(); i++) {
    nodelist.append(THIS->instancelist[i]->getValue());
  }

  SbBool ret = inherited::readInstance(in, flags);
  if (ret) {
    for (i = 1; i < THIS->instancelist.getLength(); i++) {
      SoNode * partnode = THIS->instancelist[i]->getValue();
      if (partnode != nodelist[i]) {
        partnode->ref(); // ref to make sure node is not deleted
        THIS->instancelist[i]->setValue(nodelist[i]); // restore old value
        nodelist[i] = partnode; // set value for second iteration
      }
      else nodelist[i] = NULL;
    }
    for (i = 1; i < THIS->instancelist.getLength(); i++) {
      if (nodelist[i]) { // part has changed
        this->setPart(i, nodelist[i]);
        nodelist[i]->unrefNoDelete(); // should be safe to unref now
      }
    }
  }

  (void) this->setUpConnections(oldsetup);
  (void) this->enableNotify(oldnotify);

  return ret;
}

//
// recurse until not possible to split string any more, and return information
// about part and the kit the part is found in.
// Remember to set kit=this before calling this method, also remember that
// kit might change during this search.
//
// compoundname parts are created during this search, so it might be necessary
// to do a nodekit cleanup if part is not public, or if part is set to NULL.
//
//
// if path != NULL, kit-nodes will be appended to the path during the search
// The actual part is not added to the path. The head of the path should
// be set to the kit-node performing the search.
//
SbBool
SoBaseKit::findPart(const SbString & partname, SoBaseKit *& kit, int & partnum,
                    SbBool & islist, int & listidx, const SbBool makeifneeded,
                    SoPath * path, const SbBool recsearch)
{
  // BNF:
  //
  // partname = singlename | compoundname
  // compoundname = singlename | compoundname.singlename
  // singlename = singlepartname | singlelistelementname
  // singlelistelementname = singlelistname[idx]
  //
  // singlepartname is name of a part ("ordinary", nodekit or list)
  // singlelistname is name of a part which is a list
  // idx is an integer value

  if (partname == "this") {
    islist = FALSE;
    partnum = 0;
    return TRUE;
  }

  const char * stringptr = partname.getString();
  const char * periodptr = strchr(stringptr, '.'); // find first period
  const char * startbracket = strchr(stringptr, '[');

  if (periodptr && (startbracket > periodptr))
    startbracket = NULL; // will handle later

  islist = FALSE; // set to FALSE first
  SbString firstpartname;
  if (startbracket) { // get index
    long int listindex = strtol(startbracket+1, NULL, 10);
    if (listindex == LONG_MIN || listindex == LONG_MAX) {
#if COIN_DEBUG
      SoDebugError::postWarning("SoBaseKit::findPart",
                                "list index not properly specified");
#endif // COIN_DEBUG
      return FALSE;
    }
    firstpartname = partname.getSubString(0, startbracket-stringptr-1);
    listidx = (int) listindex;
    islist = TRUE;
  }
  else if (periodptr) {
    firstpartname = partname.getSubString(0, periodptr-stringptr-1);
  }
  else firstpartname = partname;

  partnum = kit->getNodekitCatalog()->getPartNumber(firstpartname);
  if (partnum == SO_CATALOG_NAME_NOT_FOUND) {
    if (recsearch) { // search leaf nodekits for this part?
      SoBaseKit * orgkit = kit;
      assert(path == NULL); // should not do recsearch when creating path
      const SoNodekitCatalog * catalog = orgkit->getNodekitCatalog();
      for (int i = 1; i < orgkit->pimpl->instancelist.getLength(); i++) {
        if (catalog->isLeaf(i) &&
            catalog->getType(i).isDerivedFrom(SoBaseKit::getClassTypeId())) {
          kit = (SoBaseKit *) orgkit->pimpl->instancelist[i]->getValue();
          SbBool didexist = kit != NULL;
          if (!didexist) {
            orgkit->makePart(i);
            kit = (SoBaseKit *) orgkit->pimpl->instancelist[i]->getValue();
          }
          if (SoBaseKit::findPart(partname, kit, partnum, islist, listidx,
                                  makeifneeded, path, recsearch)) {
            return TRUE;
          }
          else if (!didexist) {
            // we created this part, remove it
            orgkit->setPart(i, NULL);
          }
        }
      }
      kit = orgkit; // return with an error in this kit
    }
    // nope, not found
    return FALSE;
  }

  assert(partnum < kit->pimpl->instancelist.getLength());
  SoSFNode * nodefield = kit->pimpl->instancelist[partnum];
  assert(nodefield);

  if (makeifneeded && nodefield->getValue() == NULL) {
    kit->makePart(partnum);
  }

  if (path) {
    const SoNodekitCatalog * catalog = kit->getNodekitCatalog();
    SbList <SoNode*> nodestopart;
    int parent = catalog->getParentPartNumber(partnum);
    while (parent > 0) {
      SoNode * node = kit->pimpl->instancelist[parent]->getValue();
      if (node == NULL) {
        assert(makeifneeded == FALSE);
        break;
      }
      nodestopart.push(node);
      parent = catalog->getParentPartNumber(parent);
    }
    assert(parent == 0 || !makeifneeded);
    while (nodestopart.getLength()) {
      SoNode * node = nodestopart.pop();
      path->append(node);
    }
  }

  if (periodptr == NULL) {
    // singlename or singlelistname found, do not recurse any more
    return TRUE; // all info has been found, just return TRUE
  }
  else { // recurse
    SoNode * node = nodefield->getValue();
    if (node == NULL) return FALSE;
    SbString newpartname = partname.getSubString(periodptr-stringptr+1);
    if (islist) {
      SoNodeKitListPart * list = (SoNodeKitListPart *) node;
      int numlistchildren = list->getNumChildren();
      if (listidx < 0 || listidx > numlistchildren || (!makeifneeded && listidx == numlistchildren)) {
#if COIN_DEBUG
        SoDebugError::postWarning("SoBaseKit::findPart",
                                  "index %d out of bounds for part ``%s''",
                                  listidx,
                                  firstpartname.getString());
#endif // COIN_DEBUG
        return FALSE;
      }
      else { // listidx == numlistchildren && makeifneeded
        (void) list->createAndAddDefaultChild();
      }
      SoNode * partnode = list->getChild(listidx);
      assert(partnode && partnode->isOfType(SoBaseKit::getClassTypeId()));
      kit = (SoBaseKit *)partnode;

      if (path) {
        path->append(list);
        path->append(list->getContainerNode());
      }
    }
    else {
      assert(node->isOfType(SoBaseKit::getClassTypeId()));
      kit = (SoBaseKit *)node;
    }
    if (path) path->append(kit);
    return SoBaseKit::findPart(newpartname, kit, partnum, islist,
                               listidx, makeifneeded, path, recsearch);
  }
}

//
// makes part, makes sure node is connected in the scene
//
SbBool
SoBaseKit::makePart(const int partnum)
{
  assert(partnum > 0 && partnum < THIS->instancelist.getLength());
  const SoNodekitCatalog * catalog = this->getNodekitCatalog();
  assert(catalog);
  assert(THIS->instancelist[partnum]->getValue() == NULL);

  SoNode * node = (SoNode *)catalog->getDefaultType(partnum).createInstance();
  if (catalog->isList(partnum)) {
    SoNodeKitListPart * list = (SoNodeKitListPart *) node;
    if (catalog->getListContainerType(partnum) != SoGroup::getClassTypeId()) {
      list->setContainerType(catalog->getListContainerType(partnum));
    }
    const SoTypeList & typelist = catalog->getListItemTypes(partnum);
    for (int i = 0; i < typelist.getLength(); i++) {
      list->addChildType(typelist[i]);
    }
    list->lockTypes();
  }
  return this->setPart(partnum, node);
}

/*!
  Sets parts, updates nodekit scene graph, and makes sure graph is
  valid with respect to right siblings and parent.  This method is
  virtual to enable subclasses to detect when a part changes value.

  This method is not part of the original SGI Open Inventor API, but
  is an extension specific to Coin.
*/
SbBool
SoBaseKit::setPart(const int partnum, SoNode * node)
{
  assert(partnum > 0 && partnum < THIS->instancelist.getLength());
  const SoNodekitCatalog * catalog = this->getNodekitCatalog();
  assert(catalog);

  if (node && !node->getTypeId().isDerivedFrom(catalog->getType(partnum))) {
#if COIN_DEBUG
    SoDebugError::postWarning("SoBaseKit::setPart",
                              "Attempted to set part ``%s'' "
                              "to wrong type. Expected ``%s'', got ``%s''",
                              catalog->getName(partnum).getString(),
                              catalog->getType(partnum).getName().getString(),
                              node->getTypeId().getName().getString());
#endif // COIN_DEBUG
    return FALSE;
  }
  int parentIdx = catalog->getParentPartNumber(partnum);
  assert(parentIdx >= 0 && parentIdx < THIS->instancelist.getLength());
  SoNode * parent = NULL;
  if (parentIdx == 0) parent = this;
  else parent = THIS->instancelist[parentIdx]->getValue();
  if (parent == NULL) {
    this->makePart(parentIdx);
    parent = THIS->instancelist[parentIdx]->getValue();
  }
  assert(parent != NULL);
  SoChildList * childlist = parent->getChildren();
  assert(childlist != NULL);

  SoNode * oldnode = THIS->instancelist[partnum]->getValue();
  if (oldnode == node) return TRUE; // part is already inserted

  if (childlist->find(node) >= 0) {
    // FIXME: should really allow this, but since it's a bit complex
    // (we need to somehow keep better track of which SoGroup child
    // indices belong to which catalog parts), we just disallow it for
    // now. 20020808 mortene.
    SoDebugError::postWarning("SoBaseKit::setPart",
                              "Node pointer (%p) is already used under the same group node in the catalog "
                              "as a child of %s -- this is not allowed",
                              node, catalog->getName(parentIdx).getString());
    return FALSE;
  }

  if (oldnode != NULL) { // part exists, replace
    int oldIdx = childlist->find(oldnode);
    assert(oldIdx >= 0);
    if (node) childlist->set(oldIdx, node);
    else childlist->remove(oldIdx);
  }
  else if (node) { // find where to insert in parent childlist
    int rightSibling = this->getRightSiblingIndex(partnum);
    if (rightSibling >= 0) { // part has right sibling, insert before
      int idx = childlist->find(THIS->instancelist[rightSibling]->getValue());
      assert(idx >= 0);
      childlist->insert(node, idx);
    }
    else childlist->append(node);
  }

  // set part field value
  THIS->instancelist[partnum]->setValue(node);
  return TRUE;
}

//
// returns part number of existing right sibling or -1 if none exists
//
int
SoBaseKit::getRightSiblingIndex(const int partnum)
{
  assert(partnum > 0 && partnum < THIS->instancelist.getLength());
  const SoNodekitCatalog * catalog = this->getNodekitCatalog();

  int sibling = catalog->getRightSiblingPartNumber(partnum);

  // iterate until no more siblings or until we find an existing one
  while (sibling >= 0 && THIS->instancelist[sibling]->getValue() == NULL) {
    sibling = catalog->getRightSiblingPartNumber(sibling);
  }
  return sibling;
}

//
// Searches the field list to find of a node is in this kit.
// Returns catalog index, -1 if not found
//
// parentnum is checked if >= 0
//
int
SoBaseKit::findNodeInThisKit(SoNode * node, const int parentnum) const
{
  const SoNodekitCatalog * catalog = this->getNodekitCatalog();
  if (node == (SoNode *)this) return 0;
  int n = THIS->instancelist.getLength();
  for (int i = 1; i < n; i++) {
    if (THIS->instancelist[i]->getValue() == node &&
        (parentnum < 0 || catalog->getParentPartNumber(i) == parentnum))
      return i;
  }
  return -1;
}

#undef THIS

// methods in SoBaseKitP are below

#ifndef DOXYGEN_SKIP_THIS

//
// copy the fields in kit into a new fielddata. This is done to get
// the correct write order: non-part fields first, then leaf parts,
// then non-leaf parts.
//
void
SoBaseKitP::createWriteData(void)
{
  this->writedata = new SoFieldData;
  const SoNodekitCatalog * catalog = this->kit->getNodekitCatalog();
  const SoFieldData * fielddata = kit->getFieldData();

  int n = fielddata->getNumFields();
  for (int pass = 0; pass < 3; pass++) {
    for (int i = 0; i < n; i++) {
      int part = catalog->getPartNumber(fielddata->getFieldName(i));
      // never write private parts. SGI Inventor actually exports
      // private parts in certain cases, but we feel this must be a
      // bug, so we don't do this.  pederb, 2002-02-07
      if (part > 0 && !catalog->isPublic(part)) continue;
      if ((pass == 0 && part < 0) ||
          (pass == 1 && part > 0 && catalog->isLeaf(part)) ||
          (pass == 2 && part > 0 && !catalog->isLeaf(part))) {
        this->writedata->addField(this->kit,
                                  fielddata->getFieldName(i).getString(),
                                  fielddata->getField(this->kit, i));
      }
    }
  }
}

//
// test if parent part of a part is going to write, and if so
// write part even if isDefault()
//
void
SoBaseKitP::testParentWrite(void)
{
  const SoNodekitCatalog * catalog = this->kit->getNodekitCatalog();
  int n = this->instancelist.getLength();
  for (int i = 1; i < n; i++) {
    SoSFNode * field = this->instancelist[i];
    if (field->isDefault()) { // we might not write
      SoNode * node = field->getValue();
      // don't write if NULL, of course
      if (node) {
        int parent = catalog->getParentPartNumber(i);
        if (parent > 0) {
          assert(this->writedata);
          SbName dummy;
          SoNode * parentnode = this->instancelist[parent]->getValue();
          // we must write if parent is going to write
          if (parentnode &&
              !this->instancelist[parent]->isDefault() &&
              this->writedata->getIndex(node, this->instancelist[parent]) >= 0) {            
            field->setDefault(FALSE);
          }
        }
      }
    }
  }
}

void
SoBaseKitP::copyParts(const SoBaseKit * srckit, SbList <SoNode*> & partlist,
                      const SbBool copyconnections)
{
  int i;
  const int n = this->instancelist.getLength();
  const SoNodekitCatalog * catalog = this->kit->getNodekitCatalog();

  // convenient reference
  const SbList <SoSFNode*> & srcfields = srckit->getCatalogInstances();

  // copy parts that do not have a parent as a part
  for (i = 1; i < n; i++) {
    SoNode * dstnode = this->instancelist[i]->getValue();
    if (dstnode && catalog->getParentPartNumber(i) == 0) {
      SoNode * srcnode = srcfields[i]->getValue();
      assert(dstnode != srcnode);
      dstnode->copyContents(srcnode, copyconnections);
      dstnode->ref(); // ref before inserting into list
      if (partlist[i]) partlist[i]->unref();
      partlist[i] = dstnode;
    }
  }
  // copy parts where parent is a part. These parts will already
  // have been copied, but we need to figure out the parent part node,
  // and use the correct child node as the part node instead of the
  // already copied part node.
  for (i = 1; i < n; i++) {
    int parent = catalog->getParentPartNumber(i);
    if (parent > 0 && this->instancelist[i]->getValue()) {
      SoNode * srcgroup = srcfields[parent]->getValue();
      assert(srcgroup);
      SoNode * dstgroup = partlist[parent];
      assert(dstgroup);
      assert(dstgroup->getChildren());
      assert(srcgroup->getChildren());

      // find child index in src kit
      int childidx = srcgroup->getChildren()->find(srcfields[i]->getValue());
      assert(childidx >= 0);

      // use the already copied child as part node
      assert(childidx < dstgroup->getChildren()->getLength());
      SoNode * child = (*(dstgroup->getChildren()))[childidx];
      child->ref(); // ref before inserting
      if (partlist[i]) partlist[i]->unref(); // unref old node in list
      partlist[i] = child;
    }
  }
}

void
SoBaseKitP::setParts(SbList <SoNode*> partlist, const SbBool leafparts)
{
  const int n = this->instancelist.getLength();
  const SoNodekitCatalog * catalog = this->kit->getNodekitCatalog();

  for (int i = 1; i < n; i++) {
    SoNode * node = partlist[i];
    if (node) {
      node->ref();
      SbBool leaftst = catalog->isLeaf(i);
      if (leaftst == leafparts) { // correct pass ?
        if (!leaftst) {
          // if it's not a leaf, remove children as the correct children
          // will be added  when children parts are set.
          assert(node->getChildren());
          node->getChildren()->truncate(0);
        }
        this->kit->setPart(i, node);
      }
    }
  }
}

//
// Adds a SoNodekitDetail to the picked point. path should
// contain this kit.
//
void
SoBaseKitP::addKitDetail(SoFullPath * path, SoPickedPoint * pp)
{
  const SoNodekitCatalog * catalog = this->kit->getNodekitCatalog();

  assert(path->findNode(this->kit) >= 0);

  const int n = path->getLength();
  for (int i = path->findNode(this->kit) + 1; i < n; i++) {
    SoNode * node = path->getNode(i);
    int idx = this->kit->findNodeInThisKit(node, -1);
    if (idx > 0 && catalog->isLeaf(idx)) {
      SoNodeKitDetail * detail = new SoNodeKitDetail;
      detail->setNodeKit(this->kit);
      detail->setPart(node);
      SbString partname(catalog->getName(idx));
      // check if node is a SoNodeKitListPart, and if the
      // path extends into the children. Supply index in partname
      // if this is the case.
      if (node->isOfType(SoNodeKitListPart::getClassTypeId()) &&
          path->getLength() >= i + 2) {
        SbString str;
        str.sprintf("%s[%d]",
                    partname.getString(),
                    path->getIndex(i+2));
        partname = SbName(str.getString());
      }
      detail->setPartName(partname);
      pp->setDetail(detail, this->kit);
      // finished
      break;
    }
  }
}

#endif // DOXYGEN_SKIP_THIS
