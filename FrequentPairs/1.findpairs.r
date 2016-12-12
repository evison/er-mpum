trans = read.transactions("~/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/amazonBabyRegistry/transactions.txt", format = "basket", sep=",", rm.duplicates=TRUE);
is = apriori(trans, parameter = list(support = 0.0001, minlen = 2, maxlen = 2, target = "frequent"))
summary(is)
#inspect(head(is))
#inspect(is)
#as(is, "data.frame")
#write.table(is, file = "~/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/amazonBabyRegistry/itemset.txt", sep = ",")
write(is, file="~/Documents/mypaper/x.cooperate/zhaoqi/SIGIR2016/bitbucket/sigir16/experiment/amazonBabyRegistry/itemset.txt", sep=",", quote=FALSE)
