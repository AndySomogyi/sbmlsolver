#!/usr/bin/env python

import roadrunner as rr

rr.Logger.setLevel(rr.Logger.LOG_TRACE)

def log():
    rr.Logger.log(rr.Logger.LOG_FATAL, "A fatal error. The application will most likely terminate. This is the highest priority.")
    rr.Logger.log(rr.Logger.LOG_CRITICAL, "A critical error. The application might not be able to continue running successfully.")
    rr.Logger.log(rr.Logger.LOG_ERROR, "An error. An operation did not complete successfully, but the application as a whole is not affected.")
    rr.Logger.log(rr.Logger.LOG_WARNING, "A warning. An operation completed with an unexpected result.")
    rr.Logger.log(rr.Logger.LOG_NOTICE, "A notice, which is an information with just a higher priority.")
    rr.Logger.log(rr.Logger.LOG_INFORMATION, "An informational message, usually denoting the successful completion of an operation.")
    rr.Logger.log(rr.Logger.LOG_DEBUG, "A debugging message.")
    rr.Logger.log(rr.Logger.LOG_TRACE,  "A tracing message. This is the lowest priority.")

rr.Logger.log(rr.Logger.LOG_NOTICE, "logging with default colors:")
log()

rr.Logger.setProperty("traceColor", "red")
rr.Logger.setProperty("debugColor", "green")
rr.Logger.setProperty("informationColor", "blue")
rr.Logger.setProperty("noticeColor", "magenta")
rr.Logger.setProperty("warningColor", "cyan")
rr.Logger.setProperty("errorColor", "gray")
rr.Logger.setProperty("criticalColor", "white")
rr.Logger.setProperty("fatalColor", "brown")

rr.Logger.log(rr.Logger.LOG_NOTICE, "logging with new and better colors:")
log()
