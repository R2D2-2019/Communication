## External communication
### Including this library

When including this library, add these lines to your module's `Makefile.link`:

```Makefile
#add external communication
external_comm        ?= ../../../libraries/communication
include              $(external_comm)/Makefile.inc
```

Though be sure you add those lines above these lines:
```Makefile
# Defer to the bmptk/Makefile.inc to do the work
include $(BMPTK)/Makefile.inc
```
