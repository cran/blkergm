\name{blkergm}
\alias{blkergm}
\title{
The main function to fit block ERGMs
}

\description{
This is the main function to fit block ERGMs. 
}

\usage{
blkergm(formula,offset.coef=NULL,target.stats=NULL,eval.loglik=TRUE,
estimate=c("MLE","MPLE"),control=control.ergm(),verbose=FALSE,\ldots)
}

\arguments{
\item{formula}{
The formula is expressed as nw~\link{blkergm.terms}
}

\item{offset.coef}{
See \link{ergm} for more explanation
}

\item{target.stats}{
See \link{ergm} for more explanation
}

\item{eval.loglik}{
See \link{ergm} for more explanation
}

\item{estimate}{
The estimation methods include "MLE" and "MPLE" for ERGMs, they both work for block ERGMs.
}

\item{control}{
See \link{ergm} for more explanation
}
\item{verbose}{
See \link{ergm} for more explanation
}
\item{\dots}{Additional
    arguments, to be passed to lower-level functions.
  }
}

\references{
Hunter DR, Goodreau SM, Handcock MS (2011)
{\pkg{ergm.userterms}: {A} Template Package for 
Extending \pkg{statnet}}.
}
\seealso{statnet, network, ergm, ergm-terms}
\examples{
\dontrun{
data(zachary)
summary(zachary~blkdegseq(c(0,16,34),2))
summary(zachary~blktriangle(c(16,34),2))
#block Beta model
fit <- blkergm(zachary~blkdegseq(c(0,16,34),1)+blkdegseq(c(0,16,34),2)+blkedges(c(16,34),2))
summary(fit)
}
}
\keyword{ package }
\keyword{ models }
