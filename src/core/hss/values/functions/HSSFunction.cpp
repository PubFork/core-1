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
#include "HSSFunction.h"

using namespace AXR;

HSSFunction::HSSFunction(HSSFunctionType type, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeFunction, controller)
{
    this->functionType = type;
    this->_isDirty = true;
}

HSSFunction::HSSFunction(const HSSFunction & orig)
: HSSParserNode(orig)
{
    this->functionType = orig.functionType;
    this->_isDirty = orig._isDirty;
    this->_value = orig._value;
    this->_name = orig._name;
    this->scope = orig.scope;
}

QSharedPointer<HSSFunction> HSSFunction::clone() const
{
    return qSharedPointerCast<HSSFunction> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSFunction::cloneImpl() const
{
    QSharedPointer<HSSFunction> clone = QSharedPointer<HSSFunction>(new HSSFunction(*this));

    return clone;
}

HSSFunction::~HSSFunction()
{
}

AXRString HSSFunction::toString()
{
    AXRString tempstr = AXRString("HSSFunction\n");
    return tempstr;
}

bool HSSFunction::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSFunction> castedNode = qSharedPointerCast<HSSFunction>(otherNode);
    if ( this->functionType != castedNode->functionType) return false;
    if ( this->_name != castedNode->_name) return false;
    return true;
}

QSharedPointer<HSSObject> HSSFunction::evaluate()
{
    if (this->_isDirty)
    {
        this->_isDirty = false;

        this->_value = this->_evaluate();
    }

    return this->_value;
}

QSharedPointer<HSSObject> HSSFunction::evaluate(const std::vector<QSharedPointer<HSSArgument> > & arguments)
{
    //override this
    return this->evaluate();
}

QSharedPointer<HSSObject> HSSFunction::_evaluate()
{
    //override this
    return QSharedPointer<HSSObject>();
}

void HSSFunction::propertyChanged(const AXRString property, const AXRString target, QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("__impl_private__value", theObj);
}

void HSSFunction::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    this->scope = newScope;
}

void HSSFunction::setThisObj(QSharedPointer<HSSDisplayObject> newThis)
{
    HSSParserNode::setThisObj(newThis);
}

void HSSFunction::setDirty(bool value)
{
    this->_isDirty = value;
}

bool HSSFunction::isDirty()
{
    return this->_isDirty;
}

QSharedPointer<HSSObject> HSSFunction::getValue()
{
    return this->_value;
}

bool HSSFunction::isA(HSSFunctionType type) const
{
    return this->functionType == type;
}

HSSFunctionType HSSFunction::getFunctionType() const
{
    return this->functionType;
}

void HSSFunction::setFunctionType(HSSFunctionType newType)
{
    this->functionType = newType;
}

void HSSFunction::setName(AXRString newName)
{
    this->_name = newName;
}

AXRString HSSFunction::getName()
{
    return this->_name;
}

void HSSFunction::fireTimer(AXRString timerName)
{
    //override this if needed
}
