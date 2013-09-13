\name{blkergm-package}
\alias{blkergm-package}
\docType{package}
\title{
An extension of "ergm" package for handling models with block structure
}

\details{
\tabular{ll}{
Package: \tab blkergm\cr
Type: \tab Package\cr
Version: \tab 1.0\cr
Date: \tab 2013-09-16\cr
License: \tab GPL(>= 2)\cr
}
}

\description{
This package is an extension to the package \pkg{ergm} which takes the block information into consideration for ERGMs. The implementation is based on the \pkg{ergm.userterms} package. In this package, several (block) models are considered. They include Beta model, block Erdos-Renyi, block Beta, block Markov random graph models under different assumptions. The first three models are those assuming independence between edges. The block Markov random graph models assume dependence within blocks,independence or dependence on between block edges. The package implement these models by taking different terms in the model. See \link{blkergm.terms} for explanation of those terms. Also see \pkg{statnet},\pkg{ergm} and \pkg{network} for more information on how to use ERGMs to analyse social network data.
}

\references{
Hunter DR, Goodreau SM, Handcock MS (2011)
{\link{ergm.userterms}: {A} Template Package for 
Extending \pkg{statnet}}.
}
\seealso{statnet, network, ergm, ergm-terms}
\examples{
\dontrun{
data(zachary)
summary(zachary~blktriangle(c(16,34),1)+blktriangle(c(16,34),2))
fit <- ergm(zachary~blkedges(c(16,34),1:3), MPLEonly=TRUE)
summary(fit)
}
}
\keyword{ package }
\keyword{ models }
