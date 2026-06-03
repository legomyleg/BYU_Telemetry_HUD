#include <hud/draw/telem.hpp>
#include <hud/draw/utils.hpp>

// TELEM BOX SIZES
constexpr float TELEM_SUBHEADER_SIZE = 12.0f;
constexpr float TELEM_READOUT_TEXT_SIZE = 20.0f;

void DrawTelemetryBox(const HudApp &app) {

    HudBox box = app.layout.telem;
    float margin = 10.0f;
    
    DrawRectangleRounded(box, BOX_ROUNDNESS, 8, app.colors.panelColor);
    DrawRectangleRoundedLines(box, BOX_ROUNDNESS, 8, app.colors.panelBorderColor);
    DrawTextCenteredToTop(app.hudFont, "LINK STATUS", box, BOX_HEADER_SIZE, app.colors.headerTextColor, margin);

    // Create info boxes
    float titleHeight = margin + BOX_HEADER_SIZE;
    float remHeight = box.height - titleHeight;

    float textBoxHeight = (remHeight - (margin * 4.0f)) / 3.0f;
    
    float textBoxWidth = box.width - (margin * 2.0f);
    float xVal = box.x + margin;

    float spsY = box.y + titleHeight + margin;
    Rectangle spsBox = {xVal, spsY, textBoxWidth, textBoxHeight};

    float pdY = spsY + textBoxHeight + margin;
    Rectangle pdBox = {xVal, pdY, textBoxWidth, textBoxHeight};

    float ukY = pdY + textBoxHeight + margin;
    Rectangle ukBox = {xVal, ukY, textBoxWidth, textBoxHeight};

    // SAMPLES RECEIVED PER SECOND
    float sps = app.telemetry.samples_per_sec;
    Fields::field spsText = {"SAMPLES PER SECOND", roundedStr(sps)};

    // PACKETS DROPPED
    float pd = app.telemetry.packets_dropped;
    Fields::field pdText = {"PACKETS DROPPED", roundedStr(pd)};

    float uk = 0;
    Fields::field ukText = {"TBD Value", roundedStr(uk)};

    DrawFieldsWithSubheaders(app.hudFont, spsBox, spsText, app.colors, TELEM_SUBHEADER_SIZE, TELEM_READOUT_TEXT_SIZE, 10.0f);
    DrawFieldsWithSubheaders(app.hudFont, pdBox, pdText, app.colors, TELEM_SUBHEADER_SIZE, TELEM_READOUT_TEXT_SIZE, 10.0f);
    DrawFieldsWithSubheaders(app.hudFont, ukBox, ukText, app.colors, TELEM_SUBHEADER_SIZE, TELEM_READOUT_TEXT_SIZE, 10.0f);
    
    // RUNTIME?
}
