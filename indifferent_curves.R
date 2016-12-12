b <- 2
plotted = FALSE
pdf("/Users/zhaqi/git/sigir16/writing/fig/pdf/poly_exp_contours.pdf")
for (u in 1:3) {
  x1 <- seq(1, 10, by = 0.1)
  ces.y <- ces.contour(x1,b,u)
  exp.y <- exp.contour(x1,b,u)
  ces.y[ces.y<0] <- NA
  ces.last.inf <- max(which(is.infinite(ces.y)))
  ces.y[is.infinite(ces.y)] <- NA
  ces.y[ces.last.inf] <- 20
  exp.y[exp.y<0] <- NA
  exp.last.inf <- max(which(is.infinite(exp.y)))
  exp.y[is.infinite(exp.y)] <- NA
  exp.y[exp.last.inf] <- 20
  
  if (!plotted) {
    plot(x1,ces.y,'l', col="green",xlab="", ylab="", lty = 6, lwd = 2)
    plotted = T
  } else {
    lines(x1,ces.y,'l', col="green",xlab="", ylab="", lty = 6, lwd = 2)
  }
  lines(x1,exp.y,type = 'l', col="red", lwd = 2)
}
title(main = "Indifference Curves for Polynomial and Exponential MRS", 
      xlab =expression(q[j]), ylab = expression(q[k]))
legend("topright", legend = c("Polynomial", "Exponential"), col = c("green", "red"), lty = c(6,1))
dev.off()