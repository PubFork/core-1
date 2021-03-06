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

#include <cmath>
#include "axr.h"
#include "HSSDisplayObject.h"

using namespace AXR;

HSSDisplayObject::HSSDisplayObject(HSSObjectType type, AXRController * controller)
: HSSObject(type, controller)
{
    this->_initialize();
}

void HSSDisplayObject::_initialize()
{
    this->_isDirty = true;
    this->_needsRereadRules = true;
    this->_needsSurface = true;
    this->_needsLayout = true;
    this->_rulesIndex = 0;

    this->x = this->y = this->globalX = this->globalY = 0.;

    this->_anchorXdefault = this->_anchorYdefault = true;

    this->drawIndex = 0;
    this->_index = 0;
    this->tabIndex = 1;
    this->zoomFactor = 1;
    /**
     *  @todo change to camelCase
     */
    this->does_float = false;
    this->heightByContent = false;
    this->widthByContent = false;
    this->_isRoot = false;
    this->_hasOwnWidth = false;
    this->_hasOwnHeight = false;

    this->elementName = AXRString();

    this->_isHover = false;
    this->_isPress = false;
    this->_isActive = false;
    this->_layoutFlagIsInSecondaryGroup = false;
    this->_layoutFlagIsInSecondaryLine = false;
    this->_layoutFlagVerticallyAligned = false;
    this->_layoutFlagLockTop = false;
    this->_layoutLockTopPosition = 0.;
    this->_layoutFlagLockBottom = false;
    this->_layoutLockBottomPosition = 0.;

    this->_needsFlagEvent = false;

    this->addCallback("width", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::listenWidth), new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyWidth));
    this->addCallback("height", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::listenHeight), new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyHeight));
    this->addCallback("anchorX", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeAnchorX));
    this->addNotifyCallback("anchorX", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyAnchorX));
    this->addCallback("anchorY", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeAnchorY));
    this->addNotifyCallback("anchorY", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyAnchorY));
    this->addCallback("alignX", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeAlignX));
    this->addNotifyCallback("alignX", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyAlignX));
    this->addCallback("alignY", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeAlignY));
    this->addNotifyCallback("alignY", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyAlignY));
    this->addCallback("background", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeBackground));
    this->addCallback("margin", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeMargin));
    this->addNotifyCallback("margin", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyMargin));
    this->addCallback("padding", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computePadding));
    this->addNotifyCallback("padding", new HSSObserveCallback<HSSDisplayObject>(this, &HSSDisplayObject::notifyPadding));
    this->addCallback("stroke", new HSSComputeCallback<HSSDisplayObject>(this, &HSSDisplayObject::computeStroke));
}

HSSDisplayObject::HSSDisplayObject(const HSSDisplayObject & orig)
: HSSObject(orig)
{
    this->_initialize();
    this->attributes = orig.attributes;
    this->elementName = orig.elementName;
    this->rules = orig.rules; //shallow copy
    this->_flagsStatus = orig._flagsStatus;

    this->drawIndex = orig.drawIndex;
    this->_index = orig._index;
    this->tabIndex = orig.tabIndex;
    this->heightByContent = orig.heightByContent;
    this->widthByContent = orig.widthByContent;

    this->_isHover = orig._isHover;
    this->_isPress = orig._isPress;

    this->_needsFlagEvent = orig._needsFlagEvent;
}

HSSDisplayObject::~HSSDisplayObject()
{
    axr_log(LoggerChannelGeneralSpecific, AXRString("destroying display object with name ").append(this->name));
    this->cleanTrackedObservers();
}

void HSSDisplayObject::setDefaults()
{
    this->setDefaultKw("background", "no");
    this->setDefaultKw("content", "no");
    this->setDefaultKw("on", "no");
    this->setDefaultKw("stroke", "no");
    if (this->isRoot())
    {
        this->setDefaultKw("visible", "yes");
        this->setDefaultKw("font", "no");
    }
    else
    {
        this->setDefaultPercentage("width", 100.);
        this->setDefaultKw("height", "content");
        this->setDefaultKw("anchorX", "no");
        this->setDefaultKw("anchorY", "no");
        this->setDefaultKw("flow", "yes");
        this->setDefaultKw("contained", "yes");
        this->setDefaultKw("alignX", "auto");
        this->setDefaultKw("alignY", "auto");
        this->setDefaultKw("visible", "inherit");
        this->setDefaultKw("font", "inherit");
    }
}

AXRString HSSDisplayObject::toString()
{
    if (this->isNamed())
    {
        return AXRString("HSSDisplayObject: ").append(this->name);
    }
    else
    {
        return "Anonymous HSSDisplayObject";
    }
}

AXRString HSSDisplayObject::defaultObjectType()
{
    return "displayObject";
}

HSSString HSSDisplayObject::defaultObjectType(AXRString property)
{
    //property => objtype
    static std::map<HSSString, HSSString> typeMap;
    if (typeMap.empty())
    {
        typeMap["animation"] = "transition";
        typeMap["margin"] = "margin";
        typeMap["padding"] = "margin";
    }
    
    if (typeMap.count(property) > 0)
    {
        return typeMap[property];
    }
    return HSSObject::defaultObjectType(property);
}

QSharedPointer<HSSParserNode> HSSDisplayObject::getPercentageExpression(QSharedPointer<HSSParserNode> parserNode, AXRString propertyName)
{
    if (!parserNode || !parserNode->isA(HSSParserNodeTypePercentageConstant))
    {
        AXRError("HSSDisplayObject", "You can only create a percentage expression from a percentage constant.");
        return QSharedPointer<HSSParserNode>();
    }

    static std::map<AXRString, AXRString> parentMappings;
    if (parentMappings.empty())
    {
        parentMappings["width"] = "innerWidth";
        parentMappings["height"] = "innerHeight";
        parentMappings["alignX"] = "innerWidth";
        parentMappings["alignY"] = "innerHeight";
    }

    if (parentMappings.count(propertyName))
    {
        HSSUnit number = qSharedPointerCast<HSSPercentageConstant>(parserNode)->getNumber();
        return this->getPercentageExpressionFromParent(number, parentMappings[propertyName]);
    }

    static std::map<AXRString, AXRString> thisMappings;
    if (thisMappings.empty())
    {
        thisMappings["anchorX"] = "width";
        thisMappings["anchorY"] = "height";
    }

    if (thisMappings.count(propertyName))
    {
        HSSUnit number = qSharedPointerCast<HSSPercentageConstant>(parserNode)->getNumber();
        return this->getPercentageExpressionFromThis(number, thisMappings[propertyName]);
    }

    return HSSObject::getPercentageExpression(parserNode, propertyName);
}

bool HSSDisplayObject::canHaveChildren()
{
    return false;
}

QSharedPointer<HSSContainer> HSSDisplayObject::getParent() const
{
    QSharedPointer<HSSContainer> parent = this->parent.toStrongRef();
    return parent;
}

void HSSDisplayObject::setParent(QSharedPointer<HSSContainer> parent)
{
    this->parent = pp(parent);
}

void HSSDisplayObject::removeFromParent()
{
    this->getParent()->remove(this->getIndex());
}

void HSSDisplayObject::moveToContentTree()
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    QSharedPointer<HSSDisplayObject> thisObj = this->shared_from_this();
    parent->removeFromOffscreen(thisObj);
    parent->add(thisObj);
}

void HSSDisplayObject::moveToOffscreen()
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    QSharedPointer<HSSDisplayObject> thisObj = this->shared_from_this();
    parent->addOffscreen(thisObj);
    this->removeFromParent();
}

QSharedPointer<HSSSimpleSelection> HSSDisplayObject::getSiblings()
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection(this->getController()));
    QSharedPointer<HSSSimpleSelection> children = this->getParent()->getChildren();
    for (HSSSimpleSelection::const_iterator it = children->begin(); it != children->end(); ++it)
    {
        const QSharedPointer<HSSDisplayObject> theDO = *it;
        if(theDO.data() != this)
        {
            ret->add(theDO);
        }
    }
    return ret;
}

QSharedPointer<HSSSimpleSelection> HSSDisplayObject::getNextSiblings()
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection(this->getController()));
    QSharedPointer<HSSSimpleSelection> children = this->getParent()->getChildren();
    HSSSimpleSelection::iterator thisPos = std::find(children->begin(), children->end(), this->shared_from_this());
    ret->insert(ret->end(), thisPos+1, children->end());
    return ret;
}

QSharedPointer<HSSSimpleSelection> HSSDisplayObject::getPreviousSiblings()
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection(this->getController()));
    QSharedPointer<HSSSimpleSelection> children = this->getParent()->getChildren();
    HSSSimpleSelection::iterator thisPos = std::find(children->begin(), children->end(), this->shared_from_this());
    ret->insert(ret->end(), children->begin(), thisPos);
    return ret;
}

void HSSDisplayObject::setIndex(size_t newIndex)
{
    this->_index = newIndex;
}

size_t HSSDisplayObject::getIndex() const
{
    return this->_index;
}

void HSSDisplayObject::attributesAdd(AXRString name, AXRString value)
{
    this->attributes[name] = value;
}

void HSSDisplayObject::attributesRemove(AXRString name)
{
    this->attributes.erase(name);
}

AXRString HSSDisplayObject::getElementName()
{
    return this->elementName;
}

void HSSDisplayObject::setElementName(AXRString newName)
{
    this->elementName = newName;
    this->_debugName = newName.data();
}

//rules
void HSSDisplayObject::rulesAdd(QSharedPointer<HSSRule> newRule, HSSRuleState defaultState)
{
    if (!this->hasRule(newRule))
    {
        newRule->observe("__impl_private__ruleChanged", "__impl_private__ruleAdd", this, new HSSValueChangedCallback<HSSDisplayObject > (this, &HSSDisplayObject::ruleChanged));

        newRule->appliedToAdd(this->shared_from_this());

        QSharedPointer<HSSRuleStatus> newRS = QSharedPointer<HSSRuleStatus>(new HSSRuleStatus());
        newRS->state = defaultState;
        newRS->rule = newRule;

        this->rules.push_back(newRS);
    }
}

QSharedPointer<HSSRule> HSSDisplayObject::rulesGet(size_t index) const
{
    return this->rules[index]->rule;
}

void HSSDisplayObject::rulesRemove(size_t index)
{
    QSharedPointer<HSSRule> rule = this->rules[index]->rule;
    const std::vector<QSharedPointer<HSSSelectorChain> > selectorChains = rule->getSelectorChains();
    std::vector<QSharedPointer<HSSSelectorChain> >::const_iterator it;
    for (it = selectorChains.begin(); it != selectorChains.end(); ++it)
    {
        const QSharedPointer<HSSSelectorChain> & selectorChain = *it;
        const std::deque<QSharedPointer<HSSParserNode> > nodeList = selectorChain->getNodeList();
        std::deque<QSharedPointer<HSSParserNode> >::const_iterator it2;
        for (it2 = nodeList.begin(); it2 != nodeList.end(); ++it2)
        {
            const QSharedPointer<HSSParserNode> & parserNode = *it2;
            if (parserNode->isA(HSSSelectorTypeSimpleSelector))
            {
                QSharedPointer<HSSSimpleSelector> simpleSelector = qSharedPointerCast<HSSSimpleSelector>(parserNode);
                const std::list<QSharedPointer<HSSFilter> > filters = simpleSelector->getFilters();
                std::list<QSharedPointer<HSSFilter> >::const_iterator it3;
                for (it3 = filters.begin(); it3 != filters.end(); ++it3)
                {
                    const QSharedPointer<HSSFilter> & filter = *it3;
                    if (filter->isA(HSSFilterTypeFlag))
                    {
                        QSharedPointer<HSSFlag> theFlag = qSharedPointerCast<HSSFlag>(filter);
                        this->removeFlag(theFlag);
                    }
                }
            }
        }
    }

    this->rules.erase(this->rules.begin() + index);
    this->setNeedsRereadRules(true);
}

void HSSDisplayObject::rulesRemoveLast()
{
    this->rules.pop_back();
    this->setNeedsRereadRules(true);
}

size_t HSSDisplayObject::rulesSize() const
{
    return this->rules.size();
}

void HSSDisplayObject::setRuleStatus(QSharedPointer<HSSRule> rule, HSSRuleState newValue)
{
    bool changed = false;
    bool found = false;

    std::vector<QSharedPointer<HSSRuleStatus> >::iterator it;
    for (it = this->rules.begin(); it != this->rules.end(); ++it)
    {
        QSharedPointer<HSSRuleStatus> rs = *it;
        QSharedPointer<HSSRule> existingRule = rs->rule;
        if (existingRule.data() == rule.data())
        {
            found = true;
            if (newValue == HSSRuleStatePurge)
            {
                if (rs->state == HSSRuleStateOn || rs->state == HSSRuleStateActivate)
                {
                    rs->state = newValue;
                    changed = true;
                    break;
                }
            }
            else
            {
                if (rs->state != newValue)
                {
                    rs->state = newValue;
                    changed = true;
                    break;
                }
            }
        }
    }

    if (changed)
    {
        this->setNeedsRereadRules(true);
        this->getController()->document()->setNeedsDisplay(true);
    }
}

bool HSSDisplayObject::hasRule(QSharedPointer<HSSRule> rule)
{
    bool found = false;
    std::vector<QSharedPointer<HSSRuleStatus> >::iterator it;
    for (it = this->rules.begin(); it != this->rules.end(); ++it)
    {
        QSharedPointer<HSSRuleStatus> status = *it;
        if (rule == status->rule)
        {
            found = true;
        }
    }
    return found;
}

void HSSDisplayObject::rulesInit()
{
    this->_rulesIndex = 0;
}

bool HSSDisplayObject::rulesHasNext() const
{
    return this->rulesIndex() +1 <= this->rulesSize();
}

unsigned int HSSDisplayObject::rulesIndex() const
{
    return this->_rulesIndex;
}

void HSSDisplayObject::ruleStatusStep()
{
    this->_rulesIndex += 1;
}

QSharedPointer<HSSRuleStatus> HSSDisplayObject::ruleStatusNext()
{
    QSharedPointer<HSSRuleStatus> ret;
    if(this->rulesHasNext())
    {
        ret = this->rules[this->rulesIndex()];
        this->ruleStatusStep();
    }
    //return empty pointer if no next
    return ret;
}

void HSSDisplayObject::setNeedsRereadRules(bool value)
{
    this->_needsRereadRules = value;
}

bool HSSDisplayObject::needsRereadRules()
{
    return this->_needsRereadRules;
}

void HSSDisplayObject::setNeedsSurface(bool value)
{
    this->_needsSurface = value;
}

bool HSSDisplayObject::needsSurface()
{
    return this->_needsSurface;
}

void HSSDisplayObject::setDirty(bool value)
{
    axr_log(LoggerChannelGeneralSpecific, "HSSDisplayObject: setting dirty on " + this->elementName);
    this->_isDirty = value;
}

bool HSSDisplayObject::isDirty()
{
    return this->_isDirty;
}

void HSSDisplayObject::accept(HSSAbstractVisitor* visitor, HSSVisitorFilterFlags filterFlags)
{
}

//layout
void HSSDisplayObject::setNeedsLayout(bool value)
{
    this->_needsLayout = value;
}

bool HSSDisplayObject::needsLayout() const
{
    return this->_needsLayout;
}

void HSSDisplayObject::recursiveResetLayout()
{
    this->setNeedsLayout(true);
}

void HSSDisplayObject::setGlobalX(HSSUnit newValue)
{
    this->globalX = newValue;
}

void HSSDisplayObject::setGlobalY(HSSUnit newValue)
{
    this->globalY = newValue;
}

void HSSDisplayObject::setWidth(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("width", value, specificity);
}

HSSUnit HSSDisplayObject::getWidth() const
{
    return this->getComputedNumber("width");
}

HSSUnit HSSDisplayObject::getInnerWidth() const
{
    return this->getComputedNumber("innerWidth");
}

HSSUnit HSSDisplayObject::getOuterWidth() const
{
    return this->getComputedNumber("outerWidth");
}

void HSSDisplayObject::listenWidth(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        switch (qSharedPointerCast<HSSValue>(theObj)->getValue()->getType())
        {
            case HSSParserNodeTypeExpression:
            case HSSParserNodeTypeFunction:
            case HSSParserNodeTypePropertyPath:
            {
                theObj->observe("__impl_private__valueChanged", "width", this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::widthChanged));
                break;
            }

            default:
                break;
        }
    }
}

void HSSDisplayObject::notifyWidth(QSharedPointer<HSSObject> theObj)
{
    this->widthByContent = false;
    this->_hasOwnWidth = false;
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant) && qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "content")
        {
            this->widthByContent = true;
        }
        else if (parserNode->isA(HSSParserNodeTypeNumberConstant))
        {
            this->_hasOwnWidth = true;
        }
    }
    if (this->expectsIsAIncluded())
    {
        this->_setInnerWidth(1.);
        this->_setOuterWidth(1.);
        this->setNeedsSurface(true);
        this->setNeedsLayout(true);
        if (!this->isRoot()) {
            this->getParent()->setNeedsLayout(true);
        }
        this->setDirty(true);
    }
    this->notifyObservers("width", theObj);
}

void HSSDisplayObject::widthChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    //FIXME: handle width by content
    //    if (this->widthByContent)
    //    {
    //        this->width = floor(*(HSSUnit*) data);
    //    }

    if (this->expectsIsAIncluded())
    {
        this->_setInnerWidth(1.);
        this->_setOuterWidth(1.);

        this->setNeedsSurface(true);
        this->setNeedsLayout(true);
        if (!this->isRoot()) {
            this->getParent()->setNeedsLayout(true);
        }
        this->setDirty(true);
    }
    this->notifyObservers("width", this->getComputedValue("width"));
}

void HSSDisplayObject::setHeight(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("height", value, specificity);
}

HSSUnit HSSDisplayObject::getHeight() const
{
    return this->getComputedNumber("height");
}

HSSUnit HSSDisplayObject::getInnerHeight() const
{
    return this->getComputedNumber("innerHeight");
}

HSSUnit HSSDisplayObject::getOuterHeight() const
{
    return this->getComputedNumber("outerHeight");
}

void HSSDisplayObject::listenHeight(QSharedPointer<HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeValue))
    {
        switch (qSharedPointerCast<HSSValue>(theObj)->getValue()->getType())
        {
            case HSSParserNodeTypeFunction:
            case HSSParserNodeTypeExpression:
            case HSSParserNodeTypePropertyPath:
            {
                theObj->observe("__impl_private__valueChanged", "height", this, new HSSValueChangedCallback<HSSDisplayObject>(this, &HSSDisplayObject::heightChanged));
                break;
            }

            default:
                break;
        }
    }
}

void HSSDisplayObject::notifyHeight(QSharedPointer<HSSObject> theObj)
{
    this->heightByContent = false;
    const QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        this->_hasOwnHeight = !parent->heightByContent;
        parent->setNeedsLayout(true);
    }
    else
    {
        this->_hasOwnHeight = true;
    }
    if (theObj->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant) && qSharedPointerCast<HSSKeywordConstant>(parserNode)->getValue() == "content")
        {
            this->heightByContent = true;
            this->_hasOwnHeight = true;
        }
        else if (parserNode->isA(HSSParserNodeTypeNumberConstant))
        {
            this->_hasOwnHeight = true;
        }
    }
    if (this->expectsIsAIncluded())
    {
        this->_setInnerHeight(1.);
        this->_setOuterHeight(1.);
        this->setNeedsSurface(true);
        this->setNeedsLayout(true);
        this->setDirty(true);
    }
    this->notifyObservers("height", theObj);
}

void HSSDisplayObject::heightChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    if (this->expectsIsAIncluded())
    {
        const QSharedPointer<HSSContainer> parent = this->getParent();
        if (parent)
        {
            parent->setNeedsLayout(true);
        }
        this->_setInnerHeight(1.);
        this->_setOuterHeight(1.);
        this->setNeedsSurface(true);
        this->setNeedsLayout(true);
        this->setDirty(true);
    }
    this->notifyObservers("height", this->getComputedValue("height"));
}

void HSSDisplayObject::setAnchorX(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("anchorX", value, specificity);
}

HSSUnit HSSDisplayObject::getAnchorX() const
{
    return this->getComputedNumber("anchorX");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeAnchorX(QSharedPointer<HSSParserNode> parserNode)
{
    this->_anchorXdefault = false;
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "no" || kwValue == "default")
            {
                this->_anchorXdefault = true;
            }
            else if (kwValue == "left")
            {
                return this->computeValueObject(this->percentageToConstant(0), "anchorX");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "anchorX");
            }
            else if (kwValue == "right")
            {
                return this->computeValueObject(this->percentageToConstant(100), "anchorX");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "anchorX");
}

void HSSDisplayObject::notifyAnchorX(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("anchorX", theObj);
}

void HSSDisplayObject::anchorXChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->_anchorXdefault = false;
    QSharedPointer<HSSObject> currentValue = this->getComputedValue("anchorX");
    if (currentValue->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(currentValue)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant> (parserNode);
            if (keywordValue->getValue() == "default" || keywordValue->getValue() == "no")
            {
                this->_anchorXdefault = true;
            }
        }
    }
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("anchorX", theObj);
}

void HSSDisplayObject::setAnchorY(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("anchorY", value, specificity);
}

HSSUnit HSSDisplayObject::getAnchorY() const
{
    return this->getComputedNumber("anchorY");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeAnchorY(QSharedPointer<HSSParserNode> parserNode)
{
    this->_anchorYdefault = false;
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "no" || kwValue == "default")
            {
                this->_anchorYdefault = true;
            }
            else if (kwValue == "top")
            {
                return this->computeValueObject(this->percentageToConstant(0), "anchorY");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "anchorY");
            }
            else if (kwValue == "bottom")
            {
                return this->computeValueObject(this->percentageToConstant(100), "anchorY");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "anchorY");
}

void HSSDisplayObject::notifyAnchorY(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("anchorY", theObj);
}

void HSSDisplayObject::anchorYChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    this->_anchorYdefault = false;
    QSharedPointer<HSSObject> currentValue = this->getComputedValue("anchorY");
    if (currentValue->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(currentValue)->getValue();
        if (parserNode->isA(HSSParserNodeTypeKeywordConstant))
        {
            QSharedPointer<HSSKeywordConstant> keywordValue = qSharedPointerCast<HSSKeywordConstant> (parserNode);
            if (keywordValue->getValue() == "default" || keywordValue->getValue() == "no")
            {
                this->_anchorYdefault = true;
            }
        }
    }
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("anchorY", theObj);
}


//flow
bool HSSDisplayObject::getFlow()
{
    return this->getComputedBool("flow");
}

//contained
bool HSSDisplayObject::getContained()
{
    return this->getComputedBool("contained");
}

void HSSDisplayObject::setAlignX(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("alignX", value, specificity);
}

HSSUnit HSSDisplayObject::getAlignX() const
{
    return this->getComputedNumber("alignX");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeAlignX(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "auto")
            {
                return this->inheritValue("contentAlignX", "alignX");
            }
            else if (kwValue == "left")
            {
                return this->computeValueObject(this->percentageToConstant(0), "alignX");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "alignX");
            }
            else if (kwValue == "right")
            {
                return this->computeValueObject(this->percentageToConstant(100), "alignX");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "alignX");
}

void HSSDisplayObject::notifyAlignX(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("alignX", theObj);
}

void HSSDisplayObject::alignXChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("alignX", theObj);
}

void HSSDisplayObject::setAlignY(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("alignY", value, specificity);
}

HSSUnit HSSDisplayObject::getAlignY() const
{
    return this->getComputedNumber("alignY");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeAlignY(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "auto")
            {
                return this->inheritValue("contentAlignY", "alignY");
            }
            else if (kwValue == "top")
            {
                return this->computeValueObject(this->percentageToConstant(0), "alignY");
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50), "alignY");
            }
            else if (kwValue == "bottom")
            {
                return this->computeValueObject(this->percentageToConstant(100), "alignY");
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "alignY");
}

void HSSDisplayObject::notifyAlignY(QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("alignY", theObj);
}

void HSSDisplayObject::alignYChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    QSharedPointer<HSSContainer> parent = this->getParent();
    if (parent)
    {
        parent->setNeedsLayout(true);
    }
    this->notifyObservers("alignY", theObj);
}

HSSUnit HSSDisplayObject::getLineAlign() const
{
    return this->getComputedNumber("lineAlign");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeLineAlign(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "top" || kwValue == "left")
            {
                return this->computeValueObject(this->percentageToConstant(0));
            }
            else if (kwValue == "middle" || kwValue == "center")
            {
                return this->computeValueObject(this->percentageToConstant(50));
            }
            else if (kwValue == "bottom" || kwValue == "right")
            {
                return this->computeValueObject(this->percentageToConstant(100));
            }
            break;
        }

        default:
            break;
    }
    return this->computeObject(parserNode, "lineAlign");
}

QSharedPointer<HSSObject> HSSDisplayObject::getBackground() const
{
    return this->getComputedObject("background");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeBackground(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "no")
            {
                return this->computeValueObject(parserNode);
            }
            else if (kwValue == "black")
            {
                return HSSRgb::blackColor(this->getController());
            }
            else if (kwValue == "white")
            {
                return HSSRgb::whiteColor(this->getController());
            }
            else if (kwValue == "transparent")
            {
                return HSSRgb::transparentColor(this->getController());
            }
            break;
        }

        default:
            break;
    }

    return this->computeObject(parserNode, "background");
}

//font
QSharedPointer<HSSFont> HSSDisplayObject::getFont() const
{
    QSharedPointer<HSSObject> computed = this->getComputedValue("font");;
    if (computed && computed->isA(HSSObjectTypeValue))
    {
        QSharedPointer<HSSObject> remoteObj = qSharedPointerCast<HSSValue>(computed)->getObject();
        if (remoteObj && remoteObj->isA(HSSObjectTypeFont))
        {
            return qSharedPointerCast<HSSFont>(remoteObj);
        }
    }
    else if (computed && computed->isA(HSSObjectTypeFont)){
        return qSharedPointerCast<HSSFont>(computed);
    }
    return HSSFont::defaultFont(this->getController());
}

//on
QSharedPointer<HSSObject> HSSDisplayObject::getOn() const
{
    return this->getComputedValue("on");
}

bool HSSDisplayObject::fireEvent(HSSEventType type)
{
    bool fired = false;

    QSharedPointer<HSSObject> computed = this->getComputedValue("on");;
    if (computed)
    {
        if (computed->isA(HSSObjectTypeValue))
        {
            fired = this->_fireEvent(qSharedPointerCast<HSSValue>(computed), type);
        }
        else if(computed->isA(HSSObjectTypeMultipleValue))
        {
            bool allFired = true;
            const std::vector<QSharedPointer<HSSObject> > multiValues = qSharedPointerCast<HSSMultipleValue>(computed)->getValues();
            std::vector<QSharedPointer<HSSObject> >::const_iterator it;
            for (it = multiValues.begin(); it != multiValues.end(); ++it)
            {
                const QSharedPointer<HSSObject> comp = *it;
                if (comp->isA(HSSObjectTypeValue))
                {
                    QSharedPointer<HSSValue> theValue = qSharedPointerCast<HSSValue>(comp);
                    if ( ! this->_fireEvent(theValue, type))
                    {
                        allFired = false;
                    }
                }
            }
            fired = allFired;
        }
    }

    return fired;
}

bool HSSDisplayObject::_fireEvent(QSharedPointer<HSSValue> theValue, HSSEventType eventType)
{
    bool fired = false;
    const AXRString & eventStr = HSSEvent::eventTypeToName(eventType);
    QSharedPointer<HSSObject> computed = theValue->getComputedValue(eventStr);
    if (computed)
    {
        axr_log(LoggerChannelEvents, "HSSDisplayObject: firing event: " + computed->toString());
        this->_fireEventObject(computed);
        fired = true;
    }
    return fired;
}

void HSSDisplayObject::_fireEventObject(QSharedPointer<HSSObject> theObj)
{
    if (theObj)
    {
        if (theObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(theObj)->getValue();
            this->_fireEventNode(parserNode);
        }
        else if (theObj->isA(HSSObjectTypeMultipleValue))
        {
            const std::vector<QSharedPointer<HSSObject> > multiValues = qSharedPointerCast<HSSMultipleValue>(theObj)->getValues();
            std::vector<QSharedPointer<HSSObject> >::const_iterator it;
            for (it = multiValues.begin(); it != multiValues.end(); ++it)
            {
                const QSharedPointer<HSSObject> aObj = *it;
                if (aObj->isA(HSSObjectTypeValue))
                {
                    QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(aObj)->getValue();
                    this->_fireEventNode(parserNode);
                }
            }
        }
    }
}

void HSSDisplayObject::_fireEventNode(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeFunction:
        {
            QSharedPointer<HSSFunction> theFunction = qSharedPointerCast<HSSFunction>(parserNode);
            theFunction->evaluate();
            break;
        }
        case HSSParserNodeTypePropertyPath:
        {
            QSharedPointer<HSSPropertyPath> ppath = qSharedPointerCast<HSSPropertyPath>(parserNode);
            ppath->evaluate();
            break;
        }
        default:
            break;
    }
}

//margin
QSharedPointer<HSSObject> HSSDisplayObject::getMargin() const
{
    return this->getComputedValue("margin");
}

void HSSDisplayObject::setTopMargin(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("topMargin", value, specificity);
}

HSSUnit HSSDisplayObject::getTopMargin() const
{
    return this->getComputedNumber("topMargin");
}

void HSSDisplayObject::setRightMargin(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("rightMargin", value, specificity);
}

HSSUnit HSSDisplayObject::getRightMargin() const
{
    return this->getComputedNumber("rightMargin");
}

void HSSDisplayObject::setBottomMargin(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("bottomMargin", value, specificity);
}

HSSUnit HSSDisplayObject::getBottomMargin() const
{
    return this->getComputedNumber("bottomMargin");
}

void HSSDisplayObject::setLeftMargin(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("leftMargin", value, specificity);
}

HSSUnit HSSDisplayObject::getLeftMargin() const
{
    return this->getComputedNumber("leftMargin");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeMargin(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "auto")
            {
                return QSharedPointer<HSSObject>();
            }
            break;
        }

        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> theNumber = qSharedPointerCast<HSSNumberConstant>(parserNode);
            QSharedPointer<HSSMargin> newMargin = QSharedPointer<HSSMargin>(new HSSMargin(this->getController()));
            HSSUnit specificity = parserNode->getSpecificity();
            newMargin->setSize(theNumber->getValue(), specificity);
            newMargin->setSpecificity(specificity);
            return newMargin;
            break;
        }

        default:
            break;
    }

    return this->computeObject(parserNode, "margin");
}

void HSSDisplayObject::notifyMargin(QSharedPointer<AXR::HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeMargin) && this->expectsIsAIncluded())
    {
        QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(theObj);
        HSSUnit left = theMargin->getLeft();
        HSSUnit right = theMargin->getRight();
        if (left > 0 || right > 0)
        {
            this->_setOuterWidth(1.);
        }
        HSSUnit top = theMargin->getTop();
        HSSUnit bottom = theMargin->getBottom();
        if (top > 0 || bottom > 0)
        {
            this->_setOuterHeight(1.);
        }
    }
    this->notifyObservers("margin", theObj);
}

//padding
QSharedPointer<HSSObject> HSSDisplayObject::getPadding() const
{
    return this->getComputedValue("padding");
}

QSharedPointer<HSSObject> HSSDisplayObject::computePadding(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeKeywordConstant:
        {
            QSharedPointer<HSSKeywordConstant> theKW = qSharedPointerCast<HSSKeywordConstant>(parserNode);
            AXRString kwValue = theKW->getValue();
            if (kwValue == "auto")
            {
                return QSharedPointer<HSSObject>();
            }
            break;
        }

        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> theNumber = qSharedPointerCast<HSSNumberConstant>(parserNode);
            QSharedPointer<HSSMargin> newPadding = QSharedPointer<HSSMargin>(new HSSMargin(this->getController()));
            newPadding->setSize(theNumber->getValue(), parserNode->getSpecificity());
            return newPadding;
            break;
        }

        default:
            break;
    }

    return this->computeObject(parserNode, "padding");
}

void HSSDisplayObject::notifyPadding(QSharedPointer<AXR::HSSObject> theObj)
{
    if (theObj->isA(HSSObjectTypeMargin) && this->expectsIsAIncluded())
    {
        QSharedPointer<HSSMargin> thePadding = qSharedPointerCast<HSSMargin>(theObj);
        HSSUnit left = thePadding->getLeft();
        HSSUnit right = thePadding->getRight();
        if (left > 0 || right > 0)
        {
            this->_setInnerWidth(1.);
        }
        HSSUnit top = thePadding->getTop();
        HSSUnit bottom = thePadding->getBottom();
        if (top > 0 || bottom > 0)
        {
            this->_setInnerHeight(1.);
        }
    }
    this->notifyObservers("padding", theObj);
}

void HSSDisplayObject::setTopPadding(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("topPadding", value, specificity);
}

HSSUnit HSSDisplayObject::getTopPadding() const
{
    return this->getComputedNumber("topPadding");
}

void HSSDisplayObject::setRightPadding(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("rightPadding", value, specificity);
}

HSSUnit HSSDisplayObject::getRightPadding() const
{
    return this->getComputedNumber("rightPadding");
}

void HSSDisplayObject::setBottomPadding(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("bottomPadding", value, specificity);
}

HSSUnit HSSDisplayObject::getBottomPadding() const
{
    return this->getComputedNumber("bottomPadding");
}

void HSSDisplayObject::setLeftPadding(HSSUnit value, HSSUnit specificity)
{
    this->setComputedValue("leftPadding", value, specificity);
}

HSSUnit HSSDisplayObject::getLeftPadding() const
{
    return this->getComputedNumber("leftPadding");
}

QSharedPointer<HSSObject> HSSDisplayObject::getStroke() const
{
    return this->getComputedValue("stroke");
}

QSharedPointer<HSSObject> HSSDisplayObject::computeStroke(QSharedPointer<HSSParserNode> parserNode)
{
    switch (parserNode->getType())
    {
        case HSSParserNodeTypeNumberConstant:
        {
            QSharedPointer<HSSNumberConstant> theNumber = qSharedPointerCast<HSSNumberConstant>(parserNode);
            QSharedPointer<HSSStroke> newStroke = QSharedPointer<HSSStroke>(new HSSStroke(this->getController()));
            newStroke->setSize(theNumber->getValue(), parserNode->getSpecificity());
            return newStroke;
            break;
        }

        default:
            break;
    }

    return this->computeObject(parserNode, "stroke");
}

bool HSSDisplayObject::getVisible() const
{
    return this->getComputedBool("visible");
}

void HSSDisplayObject::_setInnerWidth(HSSUnit specificity)
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit innerWidth = this->getWidth();
    HSSUnit rightPadding = 0, leftPadding = 0;
    QSharedPointer<HSSObject> paddingObj = this->getPadding();
    if (paddingObj)
    {
        if (paddingObj->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(paddingObj);
            innerWidth -= theMargin->getLeft() + theMargin->getRight();
            rightPadding += theMargin->getRight();
            leftPadding += theMargin->getLeft();
        }
        else if(paddingObj->isA(HSSObjectTypeMultipleValue))
        {
            const std::vector<QSharedPointer<HSSObject> > multiValues = qSharedPointerCast<HSSMultipleValue>(paddingObj)->getValues();
            std::vector<QSharedPointer<HSSObject> >::const_iterator it;
            for (it = multiValues.begin(); it != multiValues.end(); ++it)
            {
                const QSharedPointer<HSSObject> pObj = *it;
                if (pObj->isA(HSSObjectTypeMargin))
                {
                    QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(pObj);
                    innerWidth -= theMargin->getLeft() + theMargin->getRight();
                    rightPadding += theMargin->getRight();
                    leftPadding += theMargin->getLeft();
                }
            }
        }
        else if(paddingObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(paddingObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeNumberConstant))
            {
                HSSUnit paddingSize = qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
                rightPadding += paddingSize;
                leftPadding += paddingSize;
                innerWidth -= (paddingSize*2);
            }
        }
    }
    this->setRightPadding(rightPadding, specificity);
    this->setLeftPadding(leftPadding, specificity);
    innerWidth = std::max<HSSUnit>(0., innerWidth);
    QSharedPointer<HSSObject> computedValue = this->computeValueObject(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(innerWidth, this->getController())));
    computedValue->setSpecificity(specificity);
    this->setComputed("innerWidth", computedValue);
}

void HSSDisplayObject::_setInnerHeight(HSSUnit specificity)
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit innerHeight = this->getHeight();
    HSSUnit topPadding = 0, bottomPadding = 0;
    QSharedPointer<HSSObject> paddingObj = this->getPadding();
    if (paddingObj)
    {
        if (paddingObj->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(paddingObj);
            innerHeight -= theMargin->getTop() + theMargin->getBottom();
            topPadding += theMargin->getTop();
            bottomPadding += theMargin->getBottom();
        }
        else if(paddingObj->isA(HSSObjectTypeMultipleValue))
        {
            const std::vector<QSharedPointer<HSSObject> > multiValues = qSharedPointerCast<HSSMultipleValue>(paddingObj)->getValues();
            std::vector<QSharedPointer<HSSObject> >::const_iterator it;
            for (it = multiValues.begin(); it != multiValues.end(); ++it)
            {
                const QSharedPointer<HSSObject> pObj = *it;
                if (pObj->isA(HSSObjectTypeMargin))
                {
                    QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(pObj);
                    innerHeight -= theMargin->getTop() + theMargin->getBottom();
                    topPadding += theMargin->getTop();
                    bottomPadding += theMargin->getBottom();
                }
            }
        }
        else if(paddingObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(paddingObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeNumberConstant))
            {
                HSSUnit paddingSize = qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
                topPadding += paddingSize;
                bottomPadding += paddingSize;
                innerHeight -= (paddingSize*2);
            }
        }
    }
    this->setTopPadding(topPadding, specificity);
    this->setBottomPadding(bottomPadding, specificity);
    innerHeight = std::max<HSSUnit>(0., innerHeight);
    QSharedPointer<HSSObject> computedValue = this->computeValueObject(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(innerHeight, this->getController())));
    computedValue->setSpecificity(specificity);
    this->setComputed("innerHeight", computedValue);
}

void HSSDisplayObject::_setOuterWidth(HSSUnit specificity)
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit outerWidth = this->getWidth();
    HSSUnit rightMargin = 0, leftMargin = 0;
    QSharedPointer<HSSObject> marginObj = this->getMargin();
    if (marginObj)
    {
        if (marginObj->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(marginObj);
            outerWidth += theMargin->getLeft() + theMargin->getRight();
            rightMargin += theMargin->getRight();
            leftMargin += theMargin->getLeft();
        }
        else if(marginObj->isA(HSSObjectTypeMultipleValue))
        {
            const std::vector<QSharedPointer<HSSObject> > multiValues = qSharedPointerCast<HSSMultipleValue>(marginObj)->getValues();
            std::vector<QSharedPointer<HSSObject> >::const_iterator it;
            for (it = multiValues.begin(); it != multiValues.end(); ++it)
            {
                const QSharedPointer<HSSObject> mObj = *it;
                if (mObj->isA(HSSObjectTypeMargin))
                {
                    QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(mObj);
                    outerWidth += theMargin->getLeft() + theMargin->getRight();
                    rightMargin += theMargin->getRight();
                    leftMargin += theMargin->getLeft();
                }
            }
        }
        else if(marginObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(marginObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeNumberConstant))
            {
                HSSUnit marginSize = qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
                rightMargin += marginSize;
                leftMargin += marginSize;
                outerWidth += (marginSize*2);
            }
        }
    }
    this->setRightMargin(rightMargin, specificity);
    this->setLeftMargin(leftMargin, specificity);
    QSharedPointer<HSSObject> computedValue = this->computeValueObject(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(outerWidth, this->getController())));
    computedValue->setSpecificity(specificity);
    this->setComputed("outerWidth", computedValue);
}

void HSSDisplayObject::_setOuterHeight(HSSUnit specificity)
{
    std::vector<QSharedPointer<HSSMargin> >::const_iterator it;
    HSSUnit outerHeight = this->getHeight();
    HSSUnit topMargin = 0, bottomMargin = 0;
    QSharedPointer<HSSObject> marginObj = this->getMargin();
    if (marginObj)
    {
        if (marginObj->isA(HSSObjectTypeMargin))
        {
            QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(marginObj);
            outerHeight += theMargin->getTop() + theMargin->getBottom();
            topMargin += theMargin->getTop();
            bottomMargin += theMargin->getBottom();
        }
        else if(marginObj->isA(HSSObjectTypeMultipleValue))
        {
            const std::vector<QSharedPointer<HSSObject> > multiValues = qSharedPointerCast<HSSMultipleValue>(marginObj)->getValues();
            std::vector<QSharedPointer<HSSObject> >::const_iterator it;
            for (it = multiValues.begin(); it != multiValues.end(); ++it)
            {
                const QSharedPointer<HSSObject> mObj = *it;
                if (mObj->isA(HSSObjectTypeMargin))
                {
                    QSharedPointer<HSSMargin> theMargin = qSharedPointerCast<HSSMargin>(mObj);
                    outerHeight += theMargin->getTop() + theMargin->getBottom();
                    topMargin += theMargin->getTop();
                    bottomMargin += theMargin->getBottom();
                }
            }
        }
        else if(marginObj->isA(HSSObjectTypeValue))
        {
            QSharedPointer<HSSParserNode> parserNode = qSharedPointerCast<HSSValue>(marginObj)->getValue();
            if (parserNode->isA(HSSParserNodeTypeNumberConstant))
            {
                HSSUnit marginSize = qSharedPointerCast<HSSNumberConstant>(parserNode)->getValue();
                topMargin += marginSize;
                bottomMargin += marginSize;
                outerHeight += (marginSize*2);
            }
        }
    }
    this->setTopMargin(topMargin, specificity);
    this->setBottomMargin(bottomMargin, specificity);
    QSharedPointer<HSSObject> computedValue = this->computeValueObject(QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(outerHeight, this->getController())));
    computedValue->setSpecificity(specificity);
    this->setComputed("outerHeight", computedValue);
}

bool HSSDisplayObject::handleEvent(HSSInputEvent *event)
{
    if (!event)
        return false;

    HSSMouseEvent *mouseEvent = dynamic_cast<HSSMouseEvent*>(event);
    if (mouseEvent)
    {
        if (this->getVisible())
        {
            return this->handleMouseEvent(mouseEvent);
        }
        return false;
    }
    else
    {
        switch (event->type())
        {
            case HSSEventTypeLoad:
            case HSSEventTypeFlag:
            case HSSEventTypeUnflag:
            {
                return this->fireEvent(event->type());
            }

            case HSSEventTypeExitedWindow:
            {
                if (this->_isHover)
                {
                    this->setHover(false);
                }

                return this->fireEvent(event->type());
            }

            case HSSEventTypeKeyDown:
            case HSSEventTypeKeyUp:
            {
                return this->fireEvent(event->type());
            }

            default:
                AXRError("HSSDisplayObject", "Unknown event type").raise();
                break;
        }
    }

    return false;
}

bool HSSDisplayObject::handleMouseEvent(HSSMouseEvent *event)
{
    if (!event)
        return false;

    const HSSRect containerBounds(this->globalX, this->globalY, this->getWidth(), this->getHeight());

    switch (event->type())
    {
        case HSSEventTypeMouseDown:
        {
            if (containerBounds.contains(event->position()))
            {
                this->setPress(true);
                return this->fireEvent(event->type());
            }

            break;
        }
        case HSSEventTypeMouseUp:
        {
            this->setPress(false);

            if (containerBounds.contains(event->position()))
            {
                return this->fireEvent(event->type());
            }

            break;
        }
        case HSSEventTypeClick:
        case HSSEventTypeDoubleClick:
        {
            if (containerBounds.contains(event->position()))
                return this->fireEvent(event->type());

            break;
        }
        case HSSEventTypeMouseMove:
        {
            if (containerBounds.contains(event->position()))
            {
                if (!this->_isHover)
                {
                    this->setHover(true);
                }

                return this->fireEvent(event->type());
            }
            else if (this->_isHover)
            {
                this->setHover(false);
                return true;
            }

            break;
        }
        default:
            break;
    }

    return false;
}

void HSSDisplayObject::setHover(bool newValue)
{
    if (this->_isHover != newValue)
    {
        this->_isHover = newValue;
        if (newValue)
        {
            this->flagsActivate("hover");
        }
        else
        {
            this->flagsDeactivate("hover");
        }
    }
}

bool HSSDisplayObject::isHover()
{
    return this->_isHover;
}

void HSSDisplayObject::setPress(bool newValue)
{
    if (this->_isPress != newValue)
    {
        this->_isPress = newValue;
        if (newValue)
        {
            this->flagsActivate("press");
        }
        else
        {
            this->flagsDeactivate("press");
        }
    }
}

bool HSSDisplayObject::isPress()
{
    return this->_isPress;
}

void HSSDisplayObject::setActive(bool newValue)
{
    if (this->_isActive != newValue)
    {
        this->_isActive = newValue;
        if (newValue)
        {
            this->flagsActivate("active");
        }
        else
        {
            this->flagsDeactivate("active");
        }
    }
}

bool HSSDisplayObject::isActive()
{
    return this->_isActive;
}

void HSSDisplayObject::unsetAllActive()
{
    this->setActive(false);
}

bool HSSDisplayObject::handleSelection(HSSPoint thePoint)
{
    if (!this->getVisible())
    {
        return false;
    }
    const HSSRect containerBounds(this->globalX, this->globalY, this->getWidth(), this->getHeight());
    if (containerBounds.contains(thePoint))
    {
        this->getController()->root()->unsetAllActive();
        this->setActive(true);
        this->getController()->document()->selectionChanged(shared_from_this());
        return true;
    }
    return false;
}

void HSSDisplayObject::ruleChanged(const AXRString target, const AXRString source, const QSharedPointer<HSSObject> theObj)
{
    //HSSRule * theRule = (HSSRule*)data;
    this->setNeedsRereadRules(true);
    this->getController()->document()->setNeedsDisplay(true);
}

void HSSDisplayObject::createFlag(QSharedPointer<HSSFlag> flag, HSSRuleState defaultValue)
{
    this->_flagsStatus[flag->getName()] = defaultValue;
    this->_flags[flag->getName()].push_back(flag);
}

void HSSDisplayObject::removeFlag(QSharedPointer<HSSFlag> flag)
{
    AXRString name = flag->getName();
    if (this->hasFlag(name))
    {
        std::vector<QSharedPointer<HSSFlag> > flags = this->_flags[name];
        for (size_t i = 0; i<flags.size(); ++i)
        {
            QSharedPointer<HSSFlag> existingFlag = flags[i];
            if (existingFlag->equalTo(flag))
            {
                flags.erase(std::remove(flags.begin(), flags.end(), flag), flags.end());
            }
        }
        if (flags.size() == 0)
        {
            this->_flags.erase(name);
            this->_flagsStatus.erase(name);
        }
    }
}

bool HSSDisplayObject::hasFlag(AXRString name)
{
    if (this->_flagsStatus.find(name) != this->_flagsStatus.end())
    {
        return true;
    }
    return false;
}

HSSRuleState HSSDisplayObject::flagState(AXRString name)
{
    if (this->_flagsStatus.find(name) != this->_flagsStatus.end())
    {
        return this->_flagsStatus[name];
    }
    return HSSRuleStateOff;
}

void HSSDisplayObject::flagsActivate(AXRString name)
{
    if (this->hasFlag(name))
    {
        //std_log("activate flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = HSSRuleStateActivate;
        std::vector<QSharedPointer<HSSFlag> > flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<QSharedPointer<HSSFlag> >::iterator it;
        for (it = flags.begin(); it != flags.end(); ++it)
        {
            QSharedPointer<HSSFlag> theFlag = *it;
            theFlag->setThisObj(this->shared_from_this());
            theFlag->flagChanged(newValue);
        }
        this->_flagsStatus[name] = HSSRuleStateOn;
        this->_needsFlagEvent = true;
    }
    else
    {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

void HSSDisplayObject::flagsDeactivate(AXRString name)
{
    if (name == "*")
    {
        //std_log("deactivating all flags on element "+this->getElementName());
        for (std::map<AXRString, std::vector<QSharedPointer<HSSFlag> >  >::const_iterator it = this->_flags.begin(); it != this->_flags.end(); ++it)
        {
            HSSRuleState newValue = HSSRuleStatePurge;
            std::vector<QSharedPointer<HSSFlag> > flags = it->second;
            this->_flagsStatus[it->first] = newValue;

            for (std::vector<QSharedPointer<HSSFlag> >::iterator it = flags.begin(); it != flags.end(); ++it)
            {
                QSharedPointer<HSSFlag> theFlag = *it;
                theFlag->setThisObj(this->shared_from_this());
                theFlag->flagChanged(newValue);
            }
        }
        HSSInputEvent event(HSSEventTypeUnflag);
        this->handleEvent(&event);
    }
    else if (this->hasFlag(name))
    {
        //std_log("deactivate flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = HSSRuleStatePurge;
        std::vector<QSharedPointer<HSSFlag> > flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        for (std::vector<QSharedPointer<HSSFlag> >::iterator it = flags.begin(); it != flags.end(); ++it)
        {
            QSharedPointer<HSSFlag> theFlag = *it;
            theFlag->setThisObj(this->shared_from_this());
            theFlag->flagChanged(newValue);
        }

        this->_flagsStatus[name] = HSSRuleStateOff;
        HSSInputEvent event(HSSEventTypeUnflag);
        this->handleEvent(&event);
    }
    else
    {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

void HSSDisplayObject::flagsToggle(AXRString name)
{
    if (this->hasFlag(name))
    {
        //std_log("toggle flag with name "+name+" on element "+this->getElementName());
        HSSRuleState newValue = (this->_flagsStatus[name] == HSSRuleStateOn ? HSSRuleStatePurge : HSSRuleStateActivate);
        std::vector<QSharedPointer<HSSFlag> > flags = this->_flags[name];
        this->_flagsStatus[name] = newValue;
        std::vector<QSharedPointer<HSSFlag> >::iterator it;
        for (it = flags.begin(); it != flags.end(); ++it)
        {
            QSharedPointer<HSSFlag> theFlag = *it;
            theFlag->setThisObj(this->shared_from_this());
            theFlag->flagChanged(newValue);
        }
        this->_flagsStatus[name] = (newValue == HSSRuleStateActivate ? HSSRuleStateOn : HSSRuleStateOff);
    }
    else
    {
        //std_log("No flag with name "+name+" on element "+this->getElementName());
    }
}

bool HSSDisplayObject::isRoot()
{
    return this->_isRoot;
}

void HSSDisplayObject::setRoot(bool newValue)
{
    this->_isRoot = newValue;
}

bool HSSDisplayObject::hasOwnWidth() const
{
    return this->_hasOwnWidth;
}

void HSSDisplayObject::setHasOwnWidth(bool newValue)
{
    this->_hasOwnWidth = newValue;
}

bool HSSDisplayObject::hasOwnHeight() const
{
    return this->_hasOwnHeight;
}

void HSSDisplayObject::setHasOwnHeight(bool newValue)
{
    this->_hasOwnHeight = newValue;
}

HSSUnit HSSDisplayObject::getGlobalX() const
{
    return this->globalX;
}
HSSUnit HSSDisplayObject::getGlobalY() const
{
    return this->globalY;
}

void HSSDisplayObject::raiseFlagEventIfNeeded()
{
    if (_needsFlagEvent)
    {
        _needsFlagEvent = false;
        HSSInputEvent event(HSSEventTypeFlag);
        this->handleEvent(&event);
    }
}

QSharedPointer<HSSDisplayObject> HSSDisplayObject::shared_from_this()
{
    return qSharedPointerCast<HSSDisplayObject > (HSSObject::shared_from_this());
}
