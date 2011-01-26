#ifndef MUELU_PRFACTORY_HPP
#define MUELU_PRFACTORY_HPP

#include "MueLu_OperatorFactory.hpp"
#include "MueLu_Exceptions.hpp"

#include <iostream>

namespace MueLu {

/*!
  @class PRFactory
  @brief Factory that provides an interface for multigrid transfer operators (both restriction and prolongation).

  The user has to implement the Build function. The default implementation is GenericPRFactory.
*/

template<class ScalarType, class LocalOrdinal, class GlobalOrdinal, class Node, class LocalMatOps>
class PRFactory : public OperatorFactory<ScalarType,LocalOrdinal,GlobalOrdinal,Node, LocalMatOps> {
#include "MueLu_UseShortNames.hpp"

  private:

     GO maxCoarseSize_;
     bool reUseGraph_;
     bool reUseAggregates_;

  public:
    //! @name Constructors/Destructors.
    //@{

    //! Constructor.
    PRFactory() : maxCoarseSize_(50), reUseGraph_(false), reUseAggregates_(false)
    {
      Teuchos::OSTab tab(this->out_);
      MueLu_cout(Teuchos::VERB_HIGH) << "PRFactory: Instantiating a new factory" << std::endl;
    }

    //! Destructor.
    virtual ~PRFactory() {}
    //@}

    //! @name Build methods.
    //@{

    /*!
      @brief Abstract Build method.
    */
    bool Build(Level &fineLevel, Level &coarseLevel) = 0;
    //@}

    //! @name Set methods.
    //@{
    void SetMaxCoarseSize(GO maxCoarseSize) {
      maxCoarseSize_ = maxCoarseSize;
    }

    void ReUseAggregates(bool value) {
      reUseAggregates_ = value;
    }

    void ReUseGraph(bool value) {
      reUseGraph_ = value;
    }
    //@}

    //! @name Get methods.
    //@{

    GO GetMaxCoarseSize() {
      return maxCoarseSize_;
    }

    bool ReUseAggregates() {
      return reUseAggregates_;
    }

    bool ReUseGraph() {
      return reUseGraph_;
    }

    //@}

}; //class PRFactory

} //namespace MueLu

#define MUELU_PRFACTORY_SHORT

#endif //ifndef MUELU_PRFACTORY_HPP
