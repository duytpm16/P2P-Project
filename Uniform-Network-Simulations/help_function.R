temp <- function(xmin, xmax, ymin, ymax, r) {
xmid = (xmin + xmax) / 2
ymid = (ymin + ymax) / 2
length = abs((xmax - xmin)) / 2

left   = c(xmid, xmid - length, xmid - length, xmid)
right  = c(xmid + length, xmid, xmid, xmid  + length)
bottom = c(ymid, ymid, ymid - length, ymid - length)
top    = c(ymid + length, ymid + length, ymid, ymid)
df <- as.data.frame(cbind(left,right,bottom,top))



if (r > 1) {
  counter <- 1
  
  while (counter != sum(4^(c(1:(r-1)))) + 1) {
    xmin   <- df[counter, 1]
    xmax   <- df[counter, 2]
    ymin   <- df[counter, 3]
    ymax   <- df[counter, 4]
    
    xmid = (xmin + xmax) / 2
    ymid = (ymin + ymax) / 2
    length = abs((xmax - xmin)) / 2
    
    left   = c(xmid, xmid - length, xmid - length, xmid)
    right  = c(xmid + length, xmid, xmid, xmid  + length)
    bottom = c(ymid, ymid, ymid - length, ymid - length)
    top    = c(ymid + length, ymid + length, ymid, ymid)
    
    df <- rbind(df, cbind(left, right, bottom, top))
    counter <- counter + 1
  }
  
  df <- as.data.frame(df[-(1:sum(4^(1:(r-1)))),])
  
  return(df)
} else{
  return(df)
}
}
