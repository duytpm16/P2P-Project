library(data.table)
library(ggplot2)
library(kableExtra)

setwd("C:/Users/duytp/Desktop/P2P-Project/data/")


# Read in the network
nw <- fread("wondernetwork_scaled.txt")
colnames(nw) <- c('X', 'Y')


# Count number of nodes in each quadrant
df <- temp(0, 1, 0, 1, 3)
freq <- sapply(1:nrow(df), function(i) sum(nw$X > df$left[i] & nw$X < df$right[i] & nw$Y > df$bottom[i] & nw$Y < df$top[i]))

# Scatter plot
segs <- data.frame(x1 = c(rep(0, 9), 0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1),
                   x2 = c(rep(1, 9), 0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1),
                   y1 = c(0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, rep(0, 9)),
                   y2 = c(0, 0.125, 0.25, 0.375, 0.5, 0.625, 0.75, 0.875, 1, rep(1, 9)))

ggplot(nw, aes(x = X, y = Y)) + 
  ggtitle("Wondernetwork - 243 Nodes") +
  geom_point(color = "grey50", alpha = 0.5) +
  scale_y_continuous(breaks = c(0, 0.25, 0.5, 0.75, 1), labels = function(x) ifelse(x == 0, "0", x)) +
  scale_x_continuous(breaks = c(0, 0.25, 0.5, 0.75, 1), labels = function(x) ifelse(x == 0, "0", x))  +
  theme(panel.border = element_rect(colour = "black", fill=NA, size = 1),
        panel.background = element_blank(),
        panel.grid = element_line(size = 1, color = "grey90"),
        axis.text = element_text(size = 13, face = "bold"),
        axis.title = element_text(size = 18),
        plot.title = element_text(size = 20, hjust = 0.5)) +
  geom_segment(data = segs, aes(x = x1, xend = x2, y = y1, yend = y2), linetype = "dashed", size = 0.7)


# Bar plot
bp <- data.frame("Quadrant" = factor(paste0("q", 1:64), levels = paste0("q", 1:64)), Frequency = freq)
ggplot(bp, aes(x = Quadrant, y = Frequency)) +
  ggtitle("Distribution of Nodes") +
  geom_bar(stat = "identity", color = "black", fill = "grey90", size = 1) +
  theme(axis.line = element_line(color = "black", size = 1.3),
        panel.background = element_blank(),
        panel.grid = element_line(size = 1, color = "grey90"),
        axis.text = element_text(size = 13, face = "bold"),
        axis.text.x = element_text(angle = 90, vjust = 0.5, hjust=1),
        axis.title = element_text(size = 18),
        plot.title = element_text(size = 20, hjust = 0.5),
        legend.title = element_blank(),
        legend.text = element_text(size = 15),
        legend.position = c(0.8, 0.9),
        legend.key.size = unit(3,"line")) +
  scale_y_continuous(limits = c(0, 230)) +
  geom_hline(aes(yintercept = mean(Frequency), color = "red"), size = 1.3, linetype = "dashed") +
  scale_color_manual(labels = "Average", values = "red")



# Routing analysis
df <- data.frame(Graph = c("G*", "G"), "Routes" = c(58806, NA), `Avg Hops` = c(3.67, NA), `Max Hops` = c(52, NA),
                 `Avg Propagation` = c(0.087, NA), `Max Propagation` = c(2.21, NA))

kbl(df, col.names = c("Graph", "No. of Routes", "Avg. Hops", "Max Hops", "Avg. Propagation", "Max Propagation"),
    align = 'c') %>%
  kable_styling(bootstrap_options = "striped", full_width = F) %>%
  row_spec(c(0,2), extra_css = "border-bottom: 1px solid;") %>%
  column_spec(1, bold=T)


route <- fread("../C++/greedy_routing_results.txt", header = F)
colnames(route) <- c("x", "y")
ggplot(nw, aes(x = X, y = Y)) + 
  ggtitle("Route with Maximum Hops") +
  geom_point(color = "grey50", alpha = 0.5) +
  geom_point(data = route, aes(x = x, y = y), color = "red", size = 3) +
  geom_path(data = route, aes(x = x, y = y), size = 0.5, arrow = arrow(length = unit(0.55, "cm"))) +
  scale_y_continuous(breaks = c(0, 0.25, 0.5, 0.75, 1), labels = function(x) ifelse(x == 0, "0", x)) +
  scale_x_continuous(breaks = c(0, 0.25, 0.5, 0.75, 1), labels = function(x) ifelse(x == 0, "0", x))  +
  theme(panel.border = element_rect(colour = "black", fill=NA, size = 1),
        panel.background = element_blank(),
        panel.grid = element_line(size = 1, color = "grey90"),
        axis.text = element_text(size = 13, face = "bold"),
        axis.title = element_text(size = 18),
        plot.title = element_text(size = 20, hjust = 0.5))

p2 <- ggplot(nw, aes(x = X, y = Y)) + 
  geom_point(color = "grey50", alpha = 0.5) +
  geom_point(data = route, aes(x = x, y = y), color = "red", size = 3) +
  geom_point(aes(x = route$x[nrow(route)], y = route$y[nrow(route)]), color = "green", size = 3) +
  geom_path(data = route, aes(x = x, y = y), size = 0.5, arrow = arrow(length = unit(0.3, "cm"))) +
  scale_y_continuous(limits = c(0.38, 0.53), labels = function(x) ifelse(x == 0, "0", x)) +
  scale_x_continuous(limits = c(0.30, 0.4), labels = function(x) ifelse(x == 0, "0", x))  +
  theme(panel.border = element_rect(colour = "black", fill=NA, size = 1),
        panel.background = element_blank(),
        panel.grid = element_line(size = 1, color = "grey90"),
        axis.text = element_blank(),
        axis.title = element_blank(),
        axis.ticks = element_blank(),
        plot.title = element_text(size = 20, hjust = 0.5))

ggdraw() + draw_plot(p1) + draw_plot(p2, x = 0.55, y = 0.53, width = .44, height = .4)

