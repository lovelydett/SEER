# Gogo
Gogo is a very simple AD framework with barely no design.  
It is a student project, completely incompetent and not recommended for use in production.  
But it is already the best the author can do.  

# Design
- Single-process multi-threaded program.  
- Routines are input-bound task instances.
- Gogo schedules routines, offloads them to threads, and manages their life cycles.
- Routines consumes and emits messages.
- P/S manner of communication.
