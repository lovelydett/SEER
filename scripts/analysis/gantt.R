# Plot some gantt charts to reflect the run-time scheduling results
# Yuting@2023-3-28

# If on linux
if (Sys.info()[["sysname"]] == "Linux") {
  setwd("/home/tt/Codes/Gogo/scripts/analysis")
}

# Turn E-Epress off
options(scipen = 999)

# Set debugging
IS_DEBUG <- TRUE
DEBUG <- function(msg) {
  if (IS_DEBUG) {
    print(msg)
  }
}

load_data <- function(filename) {
  # Load data with given field names, dont add id
  df <- read.csv(filename, header = TRUE, sep = ",", stringsAsFactors = FALSE)
  return(df)
}

plot_gantt <- function(df) {
  # Plot gantt chart
  df <- load_data("gantt.csv")

  # Filter out the data with explain as "end_after_ms"
  df <- df[df$explain == "end_after_ms", ]
  DEBUG(head(df))

  # Compute the start time of each instance
  df$start_ms <- df$time_ms - df$value

  # Get the first time_ms
  start_ms <- min(df[, "start_ms"])
  df$start_ms <- df$start_ms - start_ms
  df$time_ms <- df$time_ms - start_ms

  # Plot the gantt chart
  library(ggplot2)

  gantt_task <-  qplot(xmin = start_ms,
                    xmax = time_ms,
                    y = event,
                    colour = event,
                    geom = "linerange",
                    data = df)

  gantt_core <-  qplot(xmin = start_ms,
                    xmax = time_ms,
                    y = core,
                    colour = event,
                    geom = "linerange",
                    data = df)


  # png(paste0("./result_img/", "gantt.png"), units = "in", width = 20, height = 1, res = 300)
  # save the gantt chart
  ggsave(paste0("./result_img/", "gantt_task.png"), gantt_task, width = 20, height = 5, units = "in", dpi = 300)
  ggsave(paste0("./result_img/", "gantt_core.png"), gantt_core, width = 20, height = 5, units = "in", dpi = 300)

  DEBUG(head(df))

}

plot_gantt()