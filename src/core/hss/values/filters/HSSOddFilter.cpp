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
#include "HSSOddFilter.h"

using namespace AXR;

HSSOddFilter::HSSOddFilter(AXRController * controller)
: HSSFilter(HSSFilterTypeFirst, controller)
{

}

QSharedPointer<HSSFilter> HSSOddFilter::clone() const
{
    return qSharedPointerCast<HSSOddFilter> (this->cloneImpl());
}

HSSOddFilter::~HSSOddFilter()
{

}

AXRString HSSOddFilter::toString()
{
    return "Odd Filter";
}

AXRString HSSOddFilter::stringRep()
{
    return ":odd";
}

QSharedPointer<HSSSelection> HSSOddFilter::apply(QSharedPointer<HSSSelection> scope, bool processing)
{
    QSharedPointer<HSSSimpleSelection> ret(new HSSSimpleSelection(this->getController()));
    if (scope->isA(HSSSelectionTypeMultipleSelection))
    {
        QSharedPointer<HSSMultipleSelection> multiSel = qSharedPointerCast<HSSMultipleSelection>(scope);
        for (HSSMultipleSelection::iterator it=multiSel->begin(); it!=multiSel->end(); ++it) {
            this->_apply(ret, *it);
        }
    }
    else if (scope->isA(HSSSelectionTypeSimpleSelection))
    {
        this->_apply(ret, qSharedPointerCast<HSSSimpleSelection>(scope));
    }
    return ret;
}

inline void HSSOddFilter::_apply(QSharedPointer<HSSSimpleSelection> & ret, QSharedPointer<HSSSimpleSelection> selection)
{
    unsigned i = 0;
    for (HSSSimpleSelection::const_iterator it = selection->begin(); it != selection->end(); ++it)
    {
        if (this->getNegating())
        {
            if (i % 2 != 0)
            {
                ret->add(*it);
            }
        }
        else
        {
            if (i % 2 == 0)
            {
                ret->add(*it);
            }
        }
        ++i;
    }
}

QSharedPointer<HSSClonable> HSSOddFilter::cloneImpl() const
{
    return QSharedPointer<HSSOddFilter>(new HSSOddFilter(*this));
}
