#/usr/bin/Rscript

# test MNL for utility maximization

# generate quantity assignments (candidate sets)

x <- 1:99
y <- 100 - x
b0 <- -100
z <- 1/b0 * log(x^b0 + y^b0)

data <- cbind(x,y,z)

set.seed(12345)
rnd.idx <- sample(1:length(x), size=length(x),replace = F)

# randomize the rows
data <- data[rnd.idx,]
max.idx <- which.max(data[,3])

b <- 0.1

learn.rate <- -0.02

for (i in 1:1000) {
	# quantity power
	# browser()
	qp <- data[,1:2]^b
	qp.sum <- rowSums(qp)
	log.sum <- log(qp.sum)
	# CES utility
	u <- 1/b * log(qp.sum)
	eu <- qp.sum^(1/b)
	seu <- sum(eu)
	du.db <- eu  * (-1/b^2 * log(qp.sum) + 1/b * 1/qp.sum * (rowSums(log(data[,1:2]) * qp)))

	b.grad <- (seu * du.db[max.idx] - eu[max.idx] * sum(du.db))/seu^2
	b <- b + learn.rate * b.grad
	print(sprintf("iteration:%d,b grad:%.4f, estimated power:%.3f",i,b.grad, b))
}

