#pragma once
#define PI 3.14159265359f

// ---------- Entity Offsets ----------
// Entity List
#define OFF_MODULE_ENTITY_CHAIN      0x1AB0
#define OFF_ENTITY_CHAIN_PTR1        0xAD2C
#define OFF_ENTITY_CHAIN_ENTITY_LIST 0xD7C8   // آرایه‌ی entity‌ها از نوع ENTITY_SIZE
#define ENTITY_SIZE       0x3738
#define OFF_POS_X         0x1E8
#define OFF_POS_Y         0x1EC
#define OFF_VEL_X         0x1F0
#define OFF_VEL_Y         0x1F4
#define OFF_WEAPON        0x28
#define OFF_JUMPED_TOTAL  0xA8
#define OFF_JUMPED        0xC4
#define OFF_HOOK_STATE    0xCC
#define OFF_FREEZE_END    0x100
#define OFF_IS_DEAD       0x108
#define OFF_HOOKED_PLAYER 0x1D4
#define OFF_HOOK_HIT_TYPE 0x214
#define OFF_IS_SOLO       0x310
//---------------------------------------------------------------------



// ---------- Local Player Offsets ----------
// Local Player ID
#define OFF_MODULE_ID_CHAIN      0x1A88   // moduleBase + off به یک اشاره‌گر می‌رسد
#define OFF_ID_CHAIN_PTR1        0x1734   // از آن اشاره‌گر یک پرش دیگر
#define OFF_ID_CHAIN_LOCAL_ID    0x6808   // در نهایت int32 محلی ID خوانده می‌شود
// Local Player Actions
#define OFF_MODULE_Local_BASE  0x1AA0   // اشاره‌گر به ساختار Local Actions (Activities)
#define OFF_AIM_X       0x10   // مختصات X نشانه‌روی موس (float)
#define OFF_AIM_Y       0x14   // مختصات Y نشانه‌روی موس (float)
#define OFF_AIM_JUMP   0x6C   //  (int32) – با هر بار نوشتن 1 یک پرش انجام می‌شود. برای دابل‌جامپ و پرش‌های متوالی باید 0 و سپس 1 نوشته شود.
#define OFF_AIM_SHOOT  0x70   //  (int32) – با تغییر مقدار (هر عددی) یک شلیک انجام می‌شود. روی اعداد فرد اگر ثابت بماند شلیک ادامه‌دار می‌شود (برای لیزر و شاتگان). برای تک شلیک بین دو عدد زوج حرکت دهید (مثلاً 0 ↔ 2).
#define OFF_AIM_HOOK   0x74   //  (int32) – 0 یعنی رها، 1 یعنی هوک فعال. برای هوک سریع متوالی: 1 → 0 → 1. دقت کنید هوک خودکار بعد از مدتی حتی با 1 ماندن قطع می‌شود، باید با 0 کردن و بلافاصله 1 کردن تجدید شود.
#define OFF_AIM_WEAPON 0x7C   //  (int32) – مقادیر: 1=چکش، 2=تفنگ، 3=شاتگان، 4=بمب، 5=لیزر
#define OFF_AIM_WALK_L 0x100  //  (int32) – 1 یعنی حرکت به چپ، 0 توقف
#define OFF_AIM_WALK_R 0x108  //  (int32) – 1 یعنی حرکت به راست، 0 توقف
//---------------------------------------------------------------------



// ---------- Dummy Offsets ----------

//---------------------------------------------------------------------



// ---------- Map Offsets ----------
// Map Chain
#define OFF_MODULE_MAP_CHAIN         0x1A40
#define OFF_MAP_CHAIN_PTR1           0xD98
#define OFF_MAP_CHAIN_MAP_STRUCT     0xA30   // اشاره‌گر به ساختار نقشه
// داخل ساختار نقشه:
#define OFF_MAP_WIDTH      0x60   // تعداد ستون‌های نقشه (int)
#define OFF_MAP_HEIGHT     0x64   // تعداد سطرهای نقشه (int)
#define OFF_MAP_TILES_PTR  0x68   // اشاره‌گر به آرایه‌ی تایل‌ها (4 بایت هر تایل)
