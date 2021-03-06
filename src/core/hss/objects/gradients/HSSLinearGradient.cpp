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

/**
 *  @todo set defaults
 */


#include "axr.h"
#include "HSSLinearGradient.h"

using namespace AXR;

HSSLinearGradient::HSSLinearGradient(AXRController * controller)
: HSSGradient(HSSGradientTypeLinear, controller)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: creating linear gradient object");
    this->_initialize();

    std::vector<AXRString> shorthandProperties;
    shorthandProperties.push_back("startColor");
    shorthandProperties.push_back("endColor");
    shorthandProperties.push_back("endY");
    this->setShorthandProperties(shorthandProperties);
}

HSSLinearGradient::HSSLinearGradient(const HSSLinearGradient & orig)
: HSSGradient(orig)
{
    this->_initialize();
}

void HSSLinearGradient::_initialize()
{
}

QSharedPointer<HSSLinearGradient> HSSLinearGradient::clone() const
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: cloning linear gradient object");
    return qSharedPointerCast<HSSLinearGradient> (this->cloneImpl());
}

QSharedPointer<HSSClonable> HSSLinearGradient::cloneImpl() const
{
    return QSharedPointer<HSSLinearGradient>(new HSSLinearGradient(*this));
}

HSSLinearGradient::~HSSLinearGradient()
{
    axr_log(LoggerChannelGeneralSpecific, "HSSLinearGradient: destructing linear gradient object");
    this->cleanTrackedObservers();
}

void HSSLinearGradient::setDefaults()
{
    HSSGradient::setDefaults();
    this->setDefault("startX", 0.);
    this->setDefault("startY", 0.);
    this->setDefault("endX", 0.);
    this->setDefault("endY", 0.);
}

AXRString HSSLinearGradient::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSLinearGradient: ").append(this->name);
    }
    else
    {
        return "Annonymous HSSLinearGradient";
    }
}

AXRString HSSLinearGradient::defaultObjectType()
{
    return "linearGradient";
}

QSharedPointer<HSSParserNode> HSSLinearGradient::getPercentageExpression(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (!parserNode || !parserNode->isA(HSSParserNodeTypePercentageConstant))
    {
        AXRError("HSSLinearGradient", "You can only create a percentage expression from a percentage constant.");
        return QSharedPointer<HSSParserNode>();
    }

    static std::map<AXRString, AXRString> mappings;
    if (mappings.empty())
    {
        mappings["startX"] = "width";
        mappings["startY"] = "height";
        mappings["endX"] = "width";
        mappings["endY"] = "height";
    }

    if (mappings.count(propertyName))
    {
        HSSUnit number = qSharedPointerCast<HSSPercentageConstant>(parserNode)->getNumber();
        //get the properties from itself
        return this->getPercentageExpressionFromThis(number, mappings[propertyName]);
    }

    return HSSObject::getPercentageExpression(parserNode, propertyName);
}

void HSSLinearGradient::setStartX(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("startX", value, specificity);
}

HSSUnit HSSLinearGradient::getStartX() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("startX");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

void HSSLinearGradient::setStartY(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("startY", value, specificity);
}

HSSUnit HSSLinearGradient::getStartY() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("startY");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

void HSSLinearGradient::setEndX(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("endX", value, specificity);
}

HSSUnit HSSLinearGradient::getEndX() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("endX");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}

void HSSLinearGradient::setEndY(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("endY", value, specificity);
}

HSSUnit HSSLinearGradient::getEndY() const
{
    QSharedPointer<HSSObject> value = this->getComputedValue("endY");
    if (value && value->isA(HSSObjectTypeValue))
    {
        return qSharedPointerCast<HSSValue>(value)->getNumber();
    }
    return 0.;
}
