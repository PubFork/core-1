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
#include "HSSNumberConstant.h"

using namespace AXR;

HSSNumberConstant::HSSNumberConstant(HSSUnit value, AXRController * controller)
: HSSParserNode(HSSParserNodeTypeNumberConstant, controller)
{
    this->value = value;
}

QSharedPointer<HSSNumberConstant> HSSNumberConstant::clone() const
{
    return qSharedPointerCast<HSSNumberConstant> (this->cloneImpl());
}

HSSNumberConstant::~HSSNumberConstant()
{

}

void HSSNumberConstant::setValue(HSSUnit newValue)
{
    this->value = newValue;
}

void HSSNumberConstant::setOriginalStringRep(HSSString string)
{
    this->_originalRep = string;
}

HSSString HSSNumberConstant::originalStringRep()
{
    return this->_originalRep;
}

HSSUnit HSSNumberConstant::getValue()
{
    return this->value;
}

HSSString HSSNumberConstant::toString()
{
    return HSSString::format("HSSNumberConstant with value %f", this->value);
}

HSSString HSSNumberConstant::stringRep()
{
    if (!this->_originalRep.isEmpty())
    {
        return this->_originalRep;
    }
    return HSSString::number(this->value);
}

bool HSSNumberConstant::equalTo(QSharedPointer<HSSParserNode> otherNode)
{
    //check wether pointers are the same
    if (this == otherNode.data()) return true;
    //other checks
    if ( ! HSSParserNode::equalTo(otherNode)) return false;
    QSharedPointer<HSSNumberConstant> castedNode = qSharedPointerCast<HSSNumberConstant>(otherNode);
    if ( this->value != castedNode->value ) return false;
    return true;
}

QSharedPointer<HSSClonable> HSSNumberConstant::cloneImpl() const
{
    return QSharedPointer<HSSNumberConstant>(new HSSNumberConstant(*this));
}
