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

#ifndef HSSSELECTORCHAIN_H
#define HSSSELECTORCHAIN_H

#include "HSSParserNode.h"

namespace AXR
{
    class HSSRule;
    class HSSSelector;
    class HSSSimpleSelector;

    /**
     *  @brief Encapsulates a group of related nodes as a way of "pointing" to elements
     *  in the content tree.
     *
     *  When rules are matched to display objects in the content tree, the selector chain is used
     *  to determine which ones are eligible for the rule. These instructions can range from
     *  simple selections by name, over to very complex interdepencies between elements.
     */
    class AXR_API HSSSelectorChain : public HSSParserNode
    {
    public:
        /**
         *  Iterator for vectors of shared pointers to selector chains
         */
        typedef std::vector<QSharedPointer<HSSSelectorChain> >::iterator it;

        /**
         *  Iterator for vectors of shared pointers to selector chains, const version.
         */
        typedef std::vector<QSharedPointer<HSSSelectorChain> >::const_iterator const_it;

        /**
         *  A "parent pointer", used to hold weak references to the rule that owns the selector
         *  chain, in order to break reference cycles.
         */
        typedef QWeakPointer<HSSRule> pp;

        /**
         *  Creates a new instance of a selector chain.
         */
        HSSSelectorChain(AXRController * controller);

        /**
         *  Copy constructor for HSSSelectorChain objects. Do not call directly,
         *  use clone() instead.
         */
        HSSSelectorChain(const HSSSelectorChain &orig);

        /**
         *  Clones an instance of HSSSelectorChain and gives a shared pointer of the
         *  newly instanciated object.
         *  @return A shared pointer to the new HSSSelectorChain
         */
        QSharedPointer<HSSSelectorChain> clone() const;

        /**
         *  Destructor for this class.
         */
        ~HSSSelectorChain();

        //see HSSParserNode.h for the documentation of this method
        AXRString toString();

        //see HSSParserNode.h for the documentation of this method
        AXRString stringRep();

        /**
         *  Each node overrides this method to compare against another node
         *  @param otherNode    The other object to compare to
         *  @return Wether the node is equal to the given one.
         */
        bool equalTo(QSharedPointer<HSSParserNode> otherNode);

        /**
         *  Returns a node based on the index.
         *  @param i    The index of the node in the node list.
         *  @return A shared pointer to the parser node at the given index in the node list.
         */
        const QSharedPointer<HSSParserNode> & get(size_t i) const;

        /**
         *  Adds a parser node the end of the node list.
         *  @param newNode      A shared pointer to the new parser node to be added.
         */
        void add(QSharedPointer<HSSParserNode> newNode);

        /**
         *  Adds a parser node at the beginning of the node list.
         *  @param newNode      A shared pointer to the new parser node to be added.
         */
        void prepend(QSharedPointer<HSSParserNode> newNode);

        /**
         *  Removes the last parser node in the node list.
         */
        void removeLast();

        /**
         *  @return A shared pointer to the last node in the chain.
         */
        QSharedPointer<HSSParserNode> last();

        /**
         *  @return How many nodes there are in the selector chain.
         */
        size_t size() const;

        const std::deque<QSharedPointer<HSSParserNode> > getNodeList() const;

        /**
         *  The subject of the selector chain is the last simple selector, but this can
         *  be changed with the subject selector "$" (currently unimplemented).
         *  @return A shared pointer to the subject of the selector chain.
         */
        QSharedPointer<HSSSimpleSelector> subject();

        //see HSSParserNode.h for documentation of this method
        virtual void setThisObj(QSharedPointer<HSSDisplayObject> value);
        
        /**
         *  Make the selector chain start selecting at the given selector
         */
        void fastForward(HSSSelector * theSelector);

        /**
         *  Reset the selector chain to start selecting at the beginning
         */
        void resetFastForward();

        /**
         *  @return The index at which the selection should start
         */
        const size_t getStartingSelectorIndex() const;

    protected:
        QSharedPointer<HSSSelectorChain> shared_from_this();


        std::deque<QSharedPointer<HSSParserNode> > nodeList;

    private:
        virtual QSharedPointer<HSSClonable> cloneImpl() const;
        size_t startingSelectorIndex;

        pp rule;
    };
}

#endif
