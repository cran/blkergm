\name{blkergm-terms}
\alias{blkergm-terms}
\alias{blkergm.terms}
\alias{nmedges}
\alias{nmtriangle}
\alias{nmkstar}
\alias{blkedges}
\alias{blktriangle}
\alias{blkkstar}
\alias{blktriangle2}
\alias{blkkstar2}
\alias{blktriangle3}
\alias{blkkstar3}
\alias{degseq}
\alias{blkdegseq}

\title{The terms for the ERGM package for fitting models with block structures}

\description{
This package contain R functions and terms used for fitting block ERGMs. Depending on different dependence and homogeneous assumptions on the model, the models include different terms. The package consider these assumptions and codes the terms using the "ergm.userterms" package to give an easy user interface to fit block models.The meaning and usage of these terms are listed below.
}

\section{Terms for the block ergms}{
\describe{
\item{\code{blkedges(blockbdr,blockid)}}{\emph{The number of edges in a given block with blockid:} Assume the rows and columns of the network adjacency matrix is ordered by which block they belong to. \code{blockbdr} argument is a vector specifying the boundary of blocks. For example, (5,12,17) specifies the boundary of blocks in the way that there are 3 blocks in this network and nodes 1-5 are in block 1, 6-12 are in block 2 and 13-17 are in block 3. \code{blockid} specifies the block which we want to retrieve the number of edges from. Take the above example, there are 3 blocks in that network,so that the number of between blocks is 3. They are block 1-2, block 1-3 and block 2-3. These blocks are ordered as 1,1-2,1-3,2,2-3,3. Then \code{blkedges(c(5,12,17),blockid=3)} means the number of edges in block 1-3 with block structure (5,12,17) explained above.
}

\item{\code{blktriangle(blockbdr,blockid)}}{\emph{The number of triangles in a given block with blockid:} The \code{blockbdr} is the same as that in \code{blkedges}. This term aims to return the number of triangles in a given block with \code{blockid}. Different from the \code{blockid} in \code{blkedges}, it only computes the number of triangles within a block. In the above example, there are 3 blocks. The \code{blockid} only takes value of 1,2 and 3. More specifically, \code{blktriangle((5,12,17),3)} means the number of triangles in block 3 in this network.
}

\item{\code{blkkstar(k,blockbdr,blockid)}}{\emph{The number of k-stars in a given block with blockid:} \code{blockbdr} and \code{blockid} are the same as in \code{blktriangle}.\code{k} can be a vector or scalor specifying "k"-stars. Note that, this term only return the number of k-stars within each block.
}

\item{\code{blktriangle2(blockbdr,blockid)}}{\emph{The number of triangles within each block with blockid and those formed by edges between this block and other blocks:} \code{blockid} ranges from 1 to 3 in the above example.
}

\item{\code{blkkstar2(k,blockbdr,blockid)}}{\emph{The number of k-stars within each block with blockid and those formed by edges between this block and other blocks:} \code{k} can be a vector or a scalor specifying "k"-stars. \code{blockid} ranges from 1 to 3 in the above example.
}
\item{\code{blkkstar3(k,blockbdr,blockid)}}{\emph{The number of k-stars formed by edges between two blocks} \code{blockid} can only take values for those off diagonal blocks. In the above example, blockid can take 2,3 and 5.
}

\item{\code{degseq}}{\emph{Degree sequence of a given network} The number of values returned by this term is equal to the number of nodes in the network. They correspond to the nodal degrees of all nodes.
}

\item{\code{blkdegseq(blockbdr,blockid)}}{\emph{The degree sequence in a given block with \code{blockid}:} \code{blockid} ranges from 1-3 in the above example. Please add one more 0 in front of \code{blockbdr} vector to indicate the starting position of the first node.}

}
}

\references{
Hunter DR, Goodreau SM, Handcock MS (2011)
{\pkg{ergm.userterms}: {A} Template Package for 
Extending \pkg{statnet}}.
}

\author{
Xiaolin Yang
Maintainer: Who to complain to <xyang@stat.cmu.edu>
Xiaolin Yang
}

\seealso{statnet, network, ergm, ergm-terms}
\examples{
\dontrun{
data(zachary)
summary(zachary~degseq)
summary(zachary~blkdegseq(c(0,16,34),1)+blkdegseq(c(0,16,34),2))
summary(zachary~blkedges(c(16,34),1:3))
summary(zachary~blktriangle(c(16,34),1:2))
summary(zachary~blkkstar(1:3,c(16,34),1:2))
summary(zachary~blktriangle2(c(16,34),1:2))
summary(zachary~blkkstar2(1:3,c(16,34),1:2))
summary(zachary~blkkstar3(1:3,c(16,34),2))
}
}
\keyword{ergm-terms}
\keyword{block models}
