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
 *      WEB: http://axrproject.org
 *
 *      THIS CODE AND INFORMATION ARE PROVIDED "AS IS"
 *      WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESSED
 *      OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *      IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR
 *      FITNESS FOR A PARTICULAR PURPOSE.
 *
 ********************************************************************/

#include "axr.h"
#include "HSSParserNode.h"

using namespace AXR;

AXRString HSSParserNode::parserNodeStringRepresentation(HSSParserNodeType type)
{
    static std::map<HSSParserNodeType, AXRString> types;
    if (types.empty())
    {
        types[HSSParserNodeTypeGeneric] = "HSSParserNode";
        types[HSSParserNodeTypeSelector] = "HSSSelector";
        types[HSSParserNodeTypeCombinator] = "HSSCombinator";
        types[HSSParserNodeTypeFilter] = "HSSFilter";
        types[HSSParserNodeTypeStatement] = "HSSStatement";
        types[HSSParserNodeTypeExpression] = "HSSExpression";
        types[HSSParserNodeTypeNumberConstant] = "HSSNumberConstant";
        types[HSSParserNodeTypePercentageConstant] = "HSSPercentageConstant";
        types[HSSParserNodeTypeStringConstant] = "HSSStringConstant";
        types[HSSParserNodeTypeKeywordConstant] = "HSSKeywordConstant";
        types[HSSParserNodeTypeObjectNameConstant] = "HSSObjectNameConstant";
        types[HSSParserNodeTypeFunction] = "HSSFunction";
        types[HSSParserNodeTypeMultipleValueDefinition] = "HSSParserNodeTypeMultipleValueDefinition";
        types[HSSParserNodeTypeNegation] = "HSSParserNodeTypeNegation";
        types[HSSParserNodeTypeFlag] = "HSSParserNodeTypeFlag";
    }

    AXRString ret = "";
    if (types.count(type))
    {
        ret = types[type];
    }

    return ret;
}

QSharedPointer<HSSNumberConstant> HSSParserNode::number(HSSUnit value, AXRController * controller)
{
    return QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(value, controller));
}

QSharedPointer<HSSPercentageConstant> HSSParserNode::percentage(HSSUnit value, AXRController * controller)
{
    return QSharedPointer<HSSPercentageConstant>(new HSSPercentageConstant(value, controller));
}

QSharedPointer<HSSStringConstant> HSSParserNode::string(AXRString value, AXRController * controller)
{
    return QSharedPointer<HSSStringConstant>(new HSSStringConstant(value, controller));
}

QSharedPointer<HSSKeywordConstant> HSSParserNode::keyword(AXRString value, AXRController * controller)
{
    return QSharedPointer<HSSKeywordConstant>(new HSSKeywordConstant(value, controller));
}

HSSParserNode::HSSParserNode(HSSParserNodeType type, AXRController * controller)
{
    this->nodeType = type;
    this->controller = controller;
    this->_hostProperty = "";
    this->_specificity = 0.0;
    this->_line = 0;
    this->_startCol = 0;
    this->_endCol = 0;
    this->_representedInSource = false;
}

//doesn't clone any part of the node tree, nor the observers array

HSSParserNode::HSSParserNode(const HSSParserNode &orig)
{
    this->nodeType = orig.nodeType;
    this->thisObj = orig.thisObj;
    this->controller = orig.controller;
    this->_hostProperty = orig._hostProperty;
    this->_specificity = orig._specificity;
    this->_file = orig._file;
    this->_line = orig._line;
    this->_startCol = orig._startCol;
    this->_endCol = orig._endCol;
    this->_representedInSource = orig._representedInSource;
}

QSharedPointer<HSSParserNode> HSSParserNode::clone() const
{
    return qSharedPointerCast<HSSParserNode> (this->cloneImpl());
}

AXRString HSSParserNode::toString()
{
    return "Generic parser node - you forgot to override toString in your subclass or somehow using HSSParserNode directly";
}

std::string HSSParserNode::toStdString()
{
    AXRString tempstr = this->toString();
    return tempstr.data();
}

AXRString HSSParserNode::stringRep()
{
    return "Generic parser node - you forgot to override toString in your subclass or somehow using HSSParserNode directly";
}

std::string HSSParserNode::stdStringRep()
{
    AXRString tempstr = this->stringRep();
    return tempstr.data();
}

bool HSSParserNode::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //check wether of same type
    if (otherNode->nodeType != this->nodeType) return false;
    //check wether the same amount of child nodes
    if (this->_childNodes.size() != otherNode->_childNodes.size()) return false;
    return true;
}

void HSSParserNode::setFile(QSharedPointer<AXRBuffer> file)
{
    this->_representedInSource = true;
    this->_file = file;
}

QSharedPointer<AXRBuffer> HSSParserNode::getFile() const
{
    return this->_file;
}

void HSSParserNode::setLine(long long line)
{
    this->_line = line;
}

long long HSSParserNode::getLine() const
{
    return this->_line;
}

long long HSSParserNode::getStartCol() const
{
    return this->_startCol;
}

void HSSParserNode::setStartCol(long long column)
{
    this->_startCol = column;
}

long long HSSParserNode::getEndCol() const
{
    return this->_endCol;
}

void HSSParserNode::setEndCol(long long column)
{
    this->_endCol = column;
}

bool HSSParserNode::representedInSource() const
{
    return this->_representedInSource;
}

bool HSSParserNode::isA(HSSParserNodeType otherType) const
{
    return otherType == this->nodeType;
}

HSSParserNodeType HSSParserNode::getType() const
{
    return this->nodeType;
}

QSharedPointer<HSSParserNode> HSSParserNode::getParentNode()
{
    if (!this->_parentNode.isNull())
    {
        QSharedPointer<HSSParserNode> parent = this->_parentNode.toStrongRef();
        return parent;
    }
    else
    {
        return QSharedPointer<HSSParserNode>();
    }
}

void HSSParserNode::setParentNode(QSharedPointer<HSSParserNode> newParent)
{
    this->_parentNode = QWeakPointer<HSSParserNode>(newParent);
}

void HSSParserNode::removeFromParentNode()
{
    QSharedPointer<HSSParserNode> parentNode = this->getParentNode();
    if (parentNode) parentNode->removeNode(this->shared_from_this());
}

void HSSParserNode::addNode(QSharedPointer<HSSParserNode> child)
{
    this->_childNodes.push_back(child);
}

void HSSParserNode::removeNode(QSharedPointer<HSSParserNode> child)
{
    HSSParserNode::it it = find(this->_childNodes.begin(), this->_childNodes.end(), child);
    if (it != this->_childNodes.end())
    {
        this->_childNodes.erase(it);
    }
}

std::vector<QSharedPointer<HSSParserNode> > HSSParserNode::getChildNodes() const
{
    return this->_childNodes;
}

QSharedPointer<HSSClonable> HSSParserNode::cloneImpl() const
{
    return QSharedPointer<HSSParserNode>(new HSSParserNode(*this));
}

void HSSParserNode::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    this->thisObj = value;
}

QSharedPointer<HSSDisplayObject> HSSParserNode::getThisObj() const
{
    return thisObj;
}

bool HSSParserNode::isA(HSSExpressionType otherType) const
{
    return false;
}

HSSExpressionType HSSParserNode::getExpressionType() const
{
    return HSSExpressionTypeNone;
}

bool HSSParserNode::isA(HSSUnaryExpressionType otherType) const
{
    return false;
}

HSSUnaryExpressionType HSSParserNode::getUnaryExpressionType() const
{
    return HSSUnaryExpressionTypeNone;
}

bool HSSParserNode::isA(HSSStatementType otherType) const
{
    return false;
}

HSSStatementType HSSParserNode::getStatementType() const
{
    return HSSStatementTypeNone;
}

bool HSSParserNode::isA(HSSInstructionType otherType) const
{
    return false;
}

HSSInstructionType HSSParserNode::getInstructionType() const
{
    return HSSInstructionTypeNone;
}

bool HSSParserNode::isA(HSSSelectorType otherType) const
{
    return false;
}

HSSSelectorType HSSParserNode::getSelectorType() const
{
    return HSSSelectorTypeNone;
}

bool HSSParserNode::isA(HSSCombinatorType otherType) const
{
    return false;
}

HSSCombinatorType HSSParserNode::getCombinatorType() const
{
    return HSSCombinatorTypeNone;
}

bool HSSParserNode::isA(HSSFilterType otherType) const
{
    return false;
}

HSSFilterType HSSParserNode::getFilterType() const
{
    return HSSFilterTypeNone;
}

bool HSSParserNode::isA(HSSFunctionType otherType) const
{
    return false;
}

HSSFunctionType HSSParserNode::getFunctionType() const
{
    return HSSFunctionTypeNone;
}

bool HSSParserNode::isA(HSSFlagFunctionType otherType) const
{
    return false;
}

HSSFlagFunctionType HSSParserNode::getFlagFunctionType() const
{
    return HSSFlagFunctionTypeNone;
}

QSharedPointer<HSSParserNode> HSSParserNode::shared_from_this()
{
    if (!ptr)
        ptr = QWeakPointer<HSSParserNode>(this);

    return ptr.toStrongRef();
}

AXRController* HSSParserNode::getController() const
{
    return controller;
}

void HSSParserNode::setController(AXRController *controller)
{
    this->controller = controller;
}

AXRString HSSParserNode::getHostProperty() const
{
    return this->_hostProperty;
}

void HSSParserNode::setHostProperty(AXRString newValue)
{
    this->_hostProperty = newValue;
}

HSSUnit HSSParserNode::getSpecificity() const
{
    return this->_specificity;
}

void HSSParserNode::setSpecificity(HSSUnit newValue)
{
    this->_specificity = newValue;
}

void HSSParserNode::addValue(QSharedPointer<AXR::HSSParserNode> value)
{
    //override this if needed
}

QSharedPointer<HSSObject> HSSParserNode::getVar(HSSString name)
{
    //first look at its own locals
    QSharedPointer<HSSObject> localVar = this->getVariable(name);
    if (localVar)
        return localVar;
    
    //not found, look at the parent
    QSharedPointer<HSSParserNode> parentNode = this->getParentNode();
    if (parentNode)
    {
        QSharedPointer<HSSObject> parentVar = parentNode->getVar(name);
        if (parentVar)
            return parentVar;
    }
    else
    {
        QSharedPointer<HSSObject> globalVar = this->getController()->getGlobalVariable(name);
        if (globalVar)
        {
            return globalVar;
        }
    }
    return QSharedPointer<HSSObject>();
}

QSharedPointer<HSSObject> HSSParserNode::getVariable(HSSString name)
{
    //override this if needed
    return QSharedPointer<HSSObject>();
}

bool HSSParserNode::hasLocalVar(AXR::HSSString name)
{
    //first look at its own locals
    bool hasLocal = this->hasLocalVariable(name);
    if (hasLocal)
        return hasLocal;
    
    //not found, look at the parent
    QSharedPointer<HSSParserNode> parentNode = this->getParentNode();
    if (parentNode)
    {
        bool hasParent = parentNode->hasLocalVar(name);
        if (hasParent)
            return hasParent;
    }
    return false;
}

bool HSSParserNode::hasLocalVariable(AXR::HSSString name)
{
    //override this if needed
    return false;
}

void HSSParserNode::setLocalVar(AXR::HSSString name, QSharedPointer<HSSObject> theObj)
{
    //first look at its own locals
    bool hasLocal = this->hasLocalVariable(name);
    if (hasLocal)
    {
        this->setLocalVariable(name, theObj);
        return;
    }
    
    //not found, look at the parent
    QSharedPointer<HSSParserNode> parentNode = this->getParentNode();
    if (parentNode)
    {
        parentNode->setLocalVar(name, theObj);
    }
}

void HSSParserNode::setLocalVariable(AXR::HSSString name, QSharedPointer<HSSObject> theObj)
{
    //override this if needed
    return;
}
