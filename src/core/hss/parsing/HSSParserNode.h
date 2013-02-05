/********************************************************************
 *             a  A
 *            AM\/MA
 *           (MA:MMD
 *            :: VD
 *           ::  º
 *          ::
 *         ::   **      .A$MMMMND   AMMMD     AMMM6    MMMM  MMMM6
 +       6::Z. TMMM    MMMMMMMMMDA   VMMMD   AMMM6     MMMMMMMMM6
 *      6M:AMMJMMOD     V     MMMA    VMMMD AMMM6      MMMMMMM6
 *      ::  TMMTMC         ___MMMM     VMMMMMMM6       MMMM
 *     MMM  TMMMTTM,     AMMMMMMMM      VMMMMM6        MMMM
 *    :: MM TMMTMMMD    MMMMMMMMMM       MMMMMM        MMMM
 *   ::   MMMTTMMM6    MMMMMMMMMMM      AMMMMMMD       MMMM
 *  :.     MMMMMM6    MMMM    MMMM     AMMMMMMMMD      MMMM
 *         TTMMT      MMMM    MMMM    AMMM6  MMMMD     MMMM
 *        TMMMM8       MMMMMMMMMMM   AMMM6    MMMMD    MMMM
 *       TMMMMMM$       MMMM6 MMMM  AMMM6      MMMMD   MMMM
 *      TMMM MMMM
 *     TMMM  .MMM
 *     TMM   .MMD       ARBITRARY·······XML········RENDERING
 *     TMM    MMA       ====================================
 *     TMN    MM
 *      MN    ZM
 *            MM,
 *
 *
 *      AUTHORS: see AUTHORS file
 *
 *      COPYRIGHT: ©2013 - All Rights Reserved
 *
 *      LICENSE: see LICENSE file
 *
 *      WEB: http://axr.vg
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#ifndef HSSPARSERNODE_H
#define HSSPARSERNODE_H

#include <vector>
#include <QSharedPointer>
#include <QWeakPointer>
#include "HSSClonable.h"
#include "HSSObservable.h"
#include "HSSTypeEnums.h"

namespace AXR
{
    class AXRController;
    class HSSDisplayObject;

    /**
     *  @brief Abstract base class that provides the basic functionality of a parser node.
     *
     *  When parsing an HSS file, HSSParser will create an Abstract Syntax Tree with subclasses of this class.
     *  Go to https://docs.google.com/drawings/d/1yMMwh3uddC1fFJqWt8RMShJFGhO2pq_IHEpgQYh7dKY/edit to see a diagram
     *  of the inheritance tree of parser nodes.
     */
    class AXR_API HSSParserNode : public HSSObservable, public HSSClonable, public QObject
    {
    public:
        /**
         *  Iterator for vectors of shared pointers to parser nodes.
         */
        typedef std::vector<QSharedPointer<HSSParserNode> >::iterator it;

        /**
         *  Iterator for vectors of shared pointers to parser nodes, const version.
         */
        typedef std::vector<QSharedPointer<HSSParserNode> >::const_iterator const_it;

        /**
         *  When logging, you often need a string representation of the parser node type.
         *  @param type     The parser node type to represent as a string.
         *  @return A string representation of the given type.
         */
        static AXRString parserNodeStringRepresentation(HSSParserNodeType type);

        /**
         *  Clones an instance of HSSParserNode and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSParserNode
         */
        QSharedPointer<HSSParserNode> clone() const;

        /**
         *  Prints itself as a textual representation. Each subclass should override this method.
         *  @return A textual representation of the object.
         */
        virtual AXRString toString();

        /**
         *  Prints itself as a std string representation, calling this->toString().
         *  @return A textual representation of the object, as a std string.
         */
        std::string toStdString();

        /**
         *  Call this to know if a parser node is of the given type.
         *  @param otherType    The parser node type to compare against.
         *  @return Wether the instance is an parser node of given type.
         */
        bool isA(HSSParserNodeType otherType);

        /**
         *  @return The parser node type of the current instance.
         */
        HSSParserNodeType getType();

        /**
         *  Traversal method of the parser tree.
         *  @return The parent parser node in the parser tree or an empty pointer if none.
         */
        QSharedPointer<HSSParserNode> getParentNode();

        /**
         *  Saves the given shared pointer as the parent of this instance in the parser tree.
         *  @param newParent    A shared pointer to the new parent node.
         */
        void setParentNode(QSharedPointer<HSSParserNode> newParent);

        /**
         *  Removes itself from its parent node in the parser tree.
         *
         *  @todo The saved pointer to the parent is not touched, it just removes itself on the parent.
         */
        void removeFromParentNode();

        /**
         *  Adds the given child node to the list of children
         *  @param child    A shared pointer to the child node to add.
         */
        void addNode(QSharedPointer<HSSParserNode> child);

        /**
         *  Removes the given child node from the list of children.
         *  @param child    A shared pointer to the child node to remove.
         */
        void removeNode(QSharedPointer<HSSParserNode> child);

        /**
         *  Traversal method of the parser tree.
         */
        const std::vector<QSharedPointer<HSSParserNode> > getChildNodes() const;

        /**
         *  Setter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @param value        A shared pointer to the nearest display object.
         */
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> value);
        /**
         *  Getter for the "this object", which is a shared pointer to the nearest display object
         *  (including itself).
         *  @return A shared pointer to the nearest display object.
         */
        virtual QSharedPointer<HSSDisplayObject> getThisObj();

        //add all type isAs here
        virtual bool isA(HSSExpressionType otherType);
        virtual HSSExpressionType getExpressionType();

        virtual bool isA(HSSStatementType otherType);
        virtual HSSStatementType getStatementType();

        virtual bool isA(HSSInstructionType otherType);
        virtual HSSInstructionType getInstructionType();

        virtual bool isA(HSSSelectorType otherType);
        virtual HSSSelectorType getSelectorType();

        virtual bool isA(HSSCombinatorType otherType);
        virtual HSSCombinatorType getCombinatorType();

        virtual bool isA(HSSFilterType otherType);
        virtual HSSFilterType getFilterType();

        virtual bool isA(HSSFunctionType otherType);
        virtual HSSFunctionType getFunctionType();

        virtual bool isA(HSSFlagFunctionType otherType);
        virtual HSSFlagFunctionType getFlagFunctionType();

        QSharedPointer<HSSParserNode> shared_from_this();

        /**
         *  Setter for the controller. The controller needs to be propagated across all
         *  HSSParserNode subclasses, so they get access to the DOM and such.
         *  @param controller       A pointer to the AXRController that owns this object
         */
        virtual void setController(AXRController *controller);
        /**
         *  Getter for the controller.
         *  @return A pointer to the AXRController that owns this object
         */
        virtual AXRController* getController();

    protected:
        /**
         *  Creates a new instance of a parser node. This class shouldn't be called directly,
         *  but by the subclasses.
         *  @param  type    The HSSActionType that this action corresponds to. Usually set
         *                  from withing a subclass' constructor method.
         *
         *  @todo There is no destructor on this class.
         */
        HSSParserNode(HSSParserNodeType type, AXRController * controller);

        /**
         *  Copy constructor for HSSParserNode objects. Do not call directly, use clone() instead.
         */
        HSSParserNode(const HSSParserNode &orig);

        QSharedPointer<HSSDisplayObject> thisObj;

    private:
        AXRController *controller;
        HSSParserNodeType nodeType;
        QWeakPointer<HSSParserNode> _parentNode;
        std::vector<QSharedPointer<HSSParserNode> > _childNodes;
        virtual QSharedPointer<HSSClonable> cloneImpl() const;

        QWeakPointer<HSSParserNode> ptr;
    };
}

Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSParserNode>)
Q_DECLARE_METATYPE(QSharedPointer<AXR::HSSParserNode>*)

#endif
