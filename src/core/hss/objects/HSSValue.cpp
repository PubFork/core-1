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
#include "HSSValue.h"

using namespace AXR;

QSharedPointer<HSSValue> HSSValue::valueFromParserNode(AXRController * controller, QSharedPointer<HSSParserNode> parserNode, HSSUnit specificity, QSharedPointer<HSSDisplayObject> thisObj, QSharedPointer<HSSSimpleSelection> scope)
{
    QSharedPointer<HSSValue> valueObj = QSharedPointer<HSSValue>(new HSSValue(controller));
    valueObj->setSpecificity(specificity);
    valueObj->setValue(parserNode);
    valueObj->setThisObj(thisObj);
    valueObj->setScope(scope);
    return valueObj;
}

HSSValue::HSSValue(AXRController * controller)
: HSSObject(HSSObjectTypeValue, controller)
{
    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("value");
    this->setShorthandProperties(shorthandProperties);

    this->_initialize();
}

HSSValue::HSSValue(const HSSValue & orig)
: HSSObject(orig)
{
    this->_initialize();
    if (orig.value)
    {
        this->value = orig.value->clone();
    }
}

void HSSValue::_initialize()
{

}

QSharedPointer<HSSValue> HSSValue::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSValue: cloning value object");
    return qSharedPointerCast<HSSValue>(this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSValue::cloneImpl() const
{
    return QSharedPointer<HSSValue>(new HSSValue(*this));
}

HSSValue::~HSSValue()
{
    axr_log(LoggerChannelGeneralSpecific, AXRString("HSSValue (").append(this->getHostProperty()).append("): destructing value object"));
    this->cleanTrackedObservers();
}

AXRString HSSValue::toString()
{
    return "HSSValue: " + (value ? this->value->toString() : "(null)");
}

bool HSSValue::equalTo(QSharedPointer<HSSObject> otherObj)
{
    //check wether pointers are the same
    if (this == otherObj.data()) return true;
    //other checks
    if ( ! HSSObject::equalTo(otherObj)) return false;
    QSharedPointer<HSSValue> castedObj = qSharedPointerCast<HSSValue>(otherObj);
    if (this->value && castedObj->value)
    {
        if ( ! this->value->equalTo(castedObj->value)) return false;
    }
    else if ((this->value && !castedObj->value) || (!this->value && castedObj->value))
    {
        return false;
    }
    return true;
}

AXRString HSSValue::defaultObjectType()
{
    return "value";
}

QSharedPointer<HSSParserNode> HSSValue::getValue() const
{
    return this->value;
}

void HSSValue::setValue(QSharedPointer<HSSParserNode> parserNode)
{
    this->value = parserNode;
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunction:
            parserNode->observe("__impl_private__remoteValue", "__impl_private__valueObj", this, new HSSValueChangedCallback<HSSValue>(this, &HSSValue::valueChanged));
            break;
        case HSSParserNodeTypeExpression:
            parserNode->observe("__impl_private__expressionResult", "__impl_private__valueObj", this, new HSSValueChangedCallback<HSSValue>(this, &HSSValue::valueChanged));
            break;
        case HSSParserNodeTypeStringConstant:
        {
            QSharedPointer<HSSStringConstant> theString = qSharedPointerCast<HSSStringConstant>(parserNode);
            if (theString->hasArguments())
            {
                parserNode->observe("__impl_private__remoteValue", "__impl_private__valueObj", this, new HSSValueChangedCallback<HSSValue>(this, &HSSValue::valueChanged));
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            parserNode->observe("__impl_private__remoteValue", "__impl_private__valueObj", this, new HSSValueChangedCallback<HSSValue>(this, &HSSValue::valueChanged));
        }
        default:
            break;
    }
}

void HSSValue::valueChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->notifyObservers("__impl_private__valueChanged", theObj);
}

HSSUnit HSSValue::getNumber() const
{
    return this->_getNumber(this->value);
}

HSSUnit HSSValue::_getNumber(const QSharedPointer<HSSParserNode> & parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                return this->_getNumber(qSharedPointerCast<HSSValue>(remoteObj)->getValue());
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            QSharedPointer<HSSObject> searchResult = ppath->evaluate();
            if (searchResult && searchResult->isA(HSSObjectTypeValue))
            {
                return this->_getNumber(qSharedPointerCast<HSSValue>(searchResult)->getValue());
            }
            else
            {
                AXRWarning("HSSValue", AXRString("The path ") + ppath->stringRep() + " did not yield any results.").raise();
            }
            break;
        }
        case HSSParserNodeTypeNumberConstant:
            return qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
        case HSSParserNodeTypeExpression:
            return qSharedPointerCast<HSSExpression>(parserNode)->evaluate();
        case HSSParserNodeTypeUnaryExpression:
            return qSharedPointerCast<HSSUnaryExpression>(parserNode)->evaluate();
        case HSSParserNodeTypeStringConstant:
        {
            AXRString string = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
            return string.toDouble();
        }
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kw = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (kw == "yes")
            {
                return 1.;
            }
        }
        default:
            break;
    }
    return 0.;
}

bool HSSValue::getBool() const
{
    return this->_getBool(this->value);
}

bool HSSValue::_getBool(const QSharedPointer<HSSParserNode> & parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                return this->_getBool(qSharedPointerCast<HSSValue>(remoteObj)->getValue());
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            QSharedPointer<HSSObject> searchResult = ppath->evaluate();
            if (searchResult && searchResult->isA(HSSObjectTypeValue))
            {
                return this->_getBool(qSharedPointerCast<HSSValue>(searchResult)->getValue());
            }
            else
            {
                AXRWarning("HSSValue", AXRString("The path ") + ppath->stringRep() + " did not yield any results.").raise();
            }
            break;
        }
        case HSSParserNodeTypeNumberConstant:
            return qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue() > 0.;
        case HSSParserNodeTypeExpression:
            return qSharedPointerCast<HSSExpression>(parserNode)->evaluate() > 0;
        case HSSParserNodeTypeKeywordConstant:
        {
            AXRString kwValue = qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
            if (kwValue == "yes")
            {
                return true;
            }
            break;
        }
        default:
            break;
    }
    return false;
}

AXRString HSSValue::getString() const
{
    return this->_getString(this->value);
}

AXRString HSSValue::_getString(const QSharedPointer<HSSParserNode> & parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
            if (remoteObj && remoteObj->isA(HSSObjectTypeValue))
            {
                return this->_getString(qSharedPointerCast<HSSValue>(remoteObj)->getValue());
            }
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            QSharedPointer<HSSObject> searchResult = ppath->evaluate();
            if (searchResult && searchResult->isA(HSSObjectTypeValue))
            {
                return this->_getString(qSharedPointerCast<HSSValue>(searchResult)->getValue());
            }
            else
            {
                AXRWarning("HSSValue", AXRString("The path ") + ppath->stringRep() + " did not yield any results.").raise();
            }
            break;
        }
        case HSSParserNodeTypeStringConstant:
        {
            AXRString ret = qSharedPointerCast<HSSStringConstant>(parserNode)->getValue();
            return ret.stripQuotes();
        }
        case HSSParserNodeTypeNumberConstant:
            return HSSString::number(qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue());
        case HSSParserNodeTypeExpression:
            return HSSString::number(qSharedPointerCast<HSSExpression>(parserNode)->evaluate());
        case HSSParserNodeTypeKeywordConstant:
        {
            return qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue();
        }
        default:
            break;
    }
    return "";
}

QSharedPointer<HSSObject> HSSValue::getObject() const
{
    return this->_getObject(this->value);
}

QSharedPointer<HSSObject> HSSValue::_getObject(const QSharedPointer<HSSParserNode> & parserNode) const
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunction:
        {
            return qSharedPointerCast<HSSFunction>(parserNode)->evaluate();
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            QSharedPointer<HSSObject> searchResult = ppath->evaluate();
            if (searchResult)
            {
                if (searchResult->isA(HSSObjectTypeValue))
                {
                    QSharedPointer<HSSParserNode> valueNode = qSharedPointerCast<HSSValue>(searchResult)->getValue();
                    if (valueNode)
                    {
                        return this->_getObject(valueNode);
                    }
                }
                return searchResult;
            }
            else
            {
                AXRWarning("HSSValue", AXRString("The path ") + ppath->stringRep() + " did not yield any results.").raise();
            }
            break;
        }
        default:
            break;
    }
    switch (parserNode->getStatementType()) {
        case HSSStatementTypeObjectDefinition:
        {
            QSharedPointer<HSSObjectDefinition> objDef = qSharedPointerCast<HSSObjectDefinition>(parserNode);
            QSharedPointer<HSSObject> returnObj = objDef->getObject();
            returnObj->commitStackValues();
            return returnObj;
        }
        default:
            break;
    }
    return QSharedPointer<HSSObject>();
}

void HSSValue::setScope(QSharedPointer<HSSSimpleSelection> newScope)
{
    HSSObject::setScope(newScope);
    //propagate values
    if (this->value)
    {
        switch (this->value->getType())
        {
            case HSSParserNodeTypeExpression:
                qSharedPointerCast<HSSExpression>(this->value)->setScope(newScope);
                break;
            case HSSParserNodeTypeFunction:
                qSharedPointerCast<HSSFunction>(this->value)->setScope(newScope);
                break;
            case HSSParserNodeTypeStringConstant:
                qSharedPointerCast<HSSStringConstant>(this->value)->setScope(newScope);
                break;
            case HSSParserNodeTypePropertyPath:
                qSharedPointerCast<HSSPropertyPath>(this->value)->setScope(newScope);
                break;
            default:
                break;
        }
    }
}

void HSSValue::setThisObj(QSharedPointer<HSSDisplayObject> value)
{
    HSSObject::setThisObj(value);
    //propagate values
    if (this->value)
    {
        this->value->setThisObj(value);
    }
}

void HSSValue::setHostProperty(AXRString newValue)
{
    HSSObject::setHostProperty(newValue);
    if (this->value)
    {
        this->value->setHostProperty(newValue);
    }
}

void HSSValue::replace(QSharedPointer<HSSObject> theObj)
{
    if (this->value)
    {
        switch (this->value->getType())
        {
            case HSSParserNodeTypeFunction:
            {
                if (this->value->isA(HSSFunctionTypeRef))
                {
                    QSharedPointer<HSSRefFunction> refFn = qSharedPointerCast<HSSRefFunction>(this->value);
                    refFn->replace(theObj);
                }
                break;
            }

            case HSSParserNodeTypeExpression:
            {
                QSharedPointer<HSSExpression> expObj = qSharedPointerCast<HSSExpression>(this->value);
                expObj->replace(theObj);
                break;
            }

            default:
                break;
        }
    }
    HSSObject::replace(theObj);
}
