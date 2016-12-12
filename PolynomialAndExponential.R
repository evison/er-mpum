# compare Polynomial and Exponential utility function.

ces <- function (x, y, b) {
  (x^(1-b) + y^(1-b))^(1/(1-b))
}

ces.contour <- function(x, b, u) {
  y <- (u^(1-b) - x^(1-b))^(1/(1-b))
}

exp.utility <- function(x,y,b) {
  -1/b * log(exp(-b*x) + exp(-b*y))
}

exp.contour <- function (x,b,u) {
  y <- -log(exp(-u*b) - exp(-b*x))/b
}

x <- 100
y <- 1:10

b.vals <- seq(4,70)
errs <- 1:length(b.vals)
exp.errs <- errs

for (i in 1:length(b.vals)) {
  b <- b.vals[i]
  poly.u <- ces(x,y, b)
  poly.u.min <- ces(y,y, b)
  errs[i] <- sqrt(mean((poly.u - poly.u.min)^2))
  exp.u <- exp.utility(x,y,b)
  exp.u.min <- exp.utility(y,y,b)
  exp.errs[i] <- sqrt(mean((exp.u - exp.u.min)^2))
}
pdf("/Users/zhaqi/git/sigir16/writing/fig/pdf/poly_exp_error.pdf")
plot(b.vals, errs, lty=6, type="l", lwd = 2, col = "green", ylim = c(0,1.5), xlab="", ylab="")
lines(b.vals, exp.errs, lty = 1, lwd = 2, col = "red",  ylim = c(0,1.5))
title(main="Fitting Error for Polynomial and Exponential MRS Function", xlab="b", ylab="RMSE")
legend("topleft", bty='n', legend = c("Polynomial", "Exponential"), col = c("green", "red"), lty = c(6,1))
dev.off()
