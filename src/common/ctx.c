#include <common/ctx.h>

#include <string.h>

Ctx ctx;

void ctx_init(void) {
  memset(&ctx, 0, sizeof(ctx));
}
