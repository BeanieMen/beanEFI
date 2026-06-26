#include <Uefi.h>
#include <GlobalTable.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>
#include <Protocol/GraphicsOutput.h>

static const UINT8 FontBeanie[6][8] = {
    {0x60, 0x60, 0x6E, 0x73, 0x61, 0x61, 0x73, 0x6E},
    {0x00, 0x00, 0x3C, 0x62, 0x7E, 0x60, 0x62, 0x3C},
    {0x00, 0x00, 0x3E, 0x03, 0x3F, 0x63, 0x63, 0x3F},
    {0x00, 0x00, 0x6E, 0x73, 0x61, 0x61, 0x61, 0x61},
    {0x08, 0x00, 0x18, 0x08, 0x08, 0x08, 0x08, 0x1C},
    {0x00, 0x00, 0x3C, 0x62, 0x7E, 0x60, 0x62, 0x3C}
};

#define SCALE          14
#define CHAR_WIDTH      8
#define CHAR_HEIGHT     8
#define NUM_CHARS       6
#define TEXT_WIDTH     (NUM_CHARS * CHAR_WIDTH * SCALE)
#define TEXT_HEIGHT    (CHAR_HEIGHT * SCALE)
#define BACKGROUND_CLR 0x00202080

UINT32 GetRainbowColor(UINT8 pos)
{
    UINT8 r = 0, g = 0, b = 0;

    if (pos < 85) {
        r = 255 - pos * 3;
        g = pos * 3;
    } else if (pos < 170) {
        pos -= 85;
        g = 255 - pos * 3;
        b = pos * 3;
    } else {
        pos -= 170;
        r = pos * 3;
        b = 255 - pos * 3;
    }

    return ((UINT32)b << 16) | ((UINT32)g << 8) | (UINT32)r;
}

EFI_STATUS
EFIAPI
EfiMain(
    EFI_HANDLE ImageHandle,
    EFI_SYSTEM_TABLE *SystemTable
)
{
    EFI_GUID gopGuid = EFI_GRAPHICS_OUTPUT_PROTOCOL_GUID;
    EFI_GRAPHICS_OUTPUT_PROTOCOL *gop;
    EFI_EVENT TimerEvent;
    UINTN EventIndex;

    gImageHandle = ImageHandle;
    gST = SystemTable;
    gBS = gST->BootServices;

    EFI_STATUS Status = gBS->LocateProtocol(
        &gopGuid,
        NULL,
        (void **)&gop
    );

    if (EFI_ERROR(Status))
        return Status;

    UINT32 *fb = (UINT32 *)gop->Mode->FrameBufferBase;
    UINT32 width = gop->Mode->Info->HorizontalResolution;
    UINT32 height = gop->Mode->Info->VerticalResolution;
    UINT32 pitch = gop->Mode->Info->PixelsPerScanLine;

    for (UINT32 y = 0; y < height; y++)
        for (UINT32 x = 0; x < width; x++)
            fb[y * pitch + x] = BACKGROUND_CLR;

    Status = gBS->CreateEvent(
        EVT_TIMER,
        TPL_CALLBACK,
        NULL,
        NULL,
        &TimerEvent
    );

    if (EFI_ERROR(Status))
        return Status;

    Status = gBS->SetTimer(
        TimerEvent,
        TimerPeriodic,
        166666
    );

    if (EFI_ERROR(Status))
        return Status;

    INT32 textX = (INT32)width;
    UINT32 textY = (height - TEXT_HEIGHT) / 2;
    UINT8 colorOffset = 0;

    while (1) {
        gBS->WaitForEvent(1, &TimerEvent, &EventIndex);

        for (UINT32 y = textY; y < textY + TEXT_HEIGHT; y++)
            for (UINT32 x = 0; x < width; x++)
                fb[y * pitch + x] = BACKGROUND_CLR;

        INT32 currentX = textX;

        for (UINT32 ch = 0; ch < NUM_CHARS; ch++) {
            for (UINT32 row = 0; row < CHAR_HEIGHT; row++) {
                UINT8 rowData = FontBeanie[ch][row];

                for (UINT32 col = 0; col < CHAR_WIDTH; col++) {
                    if (rowData & (0x80 >> col)) {
                        UINT8 colorPos = colorOffset + ch * 25 + col * 4;
                        UINT32 color = GetRainbowColor(colorPos);

                        for (UINT32 sy = 0; sy < SCALE; sy++) {
                            for (UINT32 sx = 0; sx < SCALE; sx++) {
                                INT32 pixelX = currentX + col * SCALE + sx;
                                UINT32 pixelY = textY + row * SCALE + sy;

                                if (pixelX >= 0 &&
                                    pixelX < (INT32)width &&
                                    pixelY < height)
                                {
                                    fb[pixelY * pitch + pixelX] = color;
                                }
                            }
                        }
                    }
                }
            }

            currentX += CHAR_WIDTH * SCALE;
        }

        textX -= 5;

        if (textX < -(INT32)TEXT_WIDTH)
            textX = (INT32)width;

        colorOffset += 3;
    }

    gBS->CloseEvent(TimerEvent);

    return EFI_SUCCESS;
}