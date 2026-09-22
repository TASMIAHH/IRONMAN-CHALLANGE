Add-Type -AssemblyName System.Drawing

function Convert-BlackToAlpha($srcPath, $dstPath) {
    $src = [System.Drawing.Bitmap]::FromFile($srcPath)
    $w = $src.Width
    $h = $src.Height
    $bmp = New-Object System.Drawing.Bitmap($w, $h, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
    
    for ($y = 0; $y -lt $h; $y++) {
        for ($x = 0; $x -lt $w; $x++) {
            $c = $src.GetPixel($x, $y)
            $br = [Math]::Max($c.R, [Math]::Max($c.G, $c.B))
            $a = [int]([Math]::Min(255, [Math]::Pow($br / 255.0, 0.85) * 255.0))
            if ($a -gt 15) {
                $bmp.SetPixel($x, $y, [System.Drawing.Color]::FromArgb($a, $c.R, $c.G, $c.B))
            } else {
                $bmp.SetPixel($x, $y, [System.Drawing.Color]::FromArgb(0, 0, 0, 0))
            }
        }
    }
    $src.Dispose()
    $bmp.Save($dstPath, [System.Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
    Write-Output "Saved $dstPath"
}

# Convert ui_play
$playJpg = "C:\Users\A\.gemini\antigravity\brain\fd8ee605-00df-42db-823a-55c98d9f6f63\ui_play_1789936581169.jpg"
if (Test-Path $playJpg) {
    Convert-BlackToAlpha $playJpg "E:\lab2\Assets\ui_play.png"
}

# 1. Generate ui_lock.png (128x128 32-bit RGBA)
$lockBmp = New-Object System.Drawing.Bitmap(128, 128, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
$g = [System.Drawing.Graphics]::FromImage($lockBmp)
$g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias

# Shackle (outer arch)
$shacklePen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(255, 190, 205, 225), 14)
$g.DrawArc($shacklePen, 34, 16, 60, 68, 180, 180)
$shacklePen.Dispose()

# Shackle highlight
$shackleHiPen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(255, 240, 248, 255), 4)
$g.DrawArc($shackleHiPen, 38, 20, 52, 60, 190, 160)
$shackleHiPen.Dispose()

# Lock Body (Titanium plate with rounded corners)
$bodyBrush = New-Object System.Drawing.Drawing2D.LinearGradientBrush(
    (New-Object System.Drawing.Point(0, 48)),
    (New-Object System.Drawing.Point(0, 114)),
    [System.Drawing.Color]::FromArgb(255, 75, 88, 108),
    [System.Drawing.Color]::FromArgb(255, 28, 35, 48)
)
$path = New-Object System.Drawing.Drawing2D.GraphicsPath
$path.AddArc(24, 48, 16, 16, 180, 90)
$path.AddArc(88, 48, 16, 16, 270, 90)
$path.AddArc(88, 98, 16, 16, 0, 90)
$path.AddArc(24, 98, 16, 16, 90, 90)
$path.CloseFigure()
$g.FillPath($bodyBrush, $path)
$bodyBrush.Dispose()

# Gold Rim
$goldPen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(255, 255, 204, 51), 3)
$g.DrawPath($goldPen, $path)
$goldPen.Dispose()
$path.Dispose()

# Cyan LED Accent line
$ledPen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(255, 0, 220, 255), 2)
$g.DrawLine($ledPen, 36, 56, 92, 56)
$ledPen.Dispose()

# Keyhole (dark center with glow)
$khBrush = New-Object System.Drawing.SolidBrush([System.Drawing.Color]::FromArgb(255, 12, 16, 24))
$g.FillEllipse($khBrush, 58, 68, 12, 12)
$khPoly = @(
    (New-Object System.Drawing.Point(60, 74)),
    (New-Object System.Drawing.Point(68, 74)),
    (New-Object System.Drawing.Point(71, 92)),
    (New-Object System.Drawing.Point(57, 92))
)
$g.FillPolygon($khBrush, $khPoly)
$khBrush.Dispose()

$g.Dispose()
$lockBmp.Save("E:\lab2\Assets\ui_lock.png", [System.Drawing.Imaging.ImageFormat]::Png)
$lockBmp.Dispose()
Write-Output "Generated ui_lock.png"

# 2. Generate Medals (Gold, Silver, Bronze)
function Make-Medal($color1, $color2, $rimColor, $ribbonColor, $fileName) {
    $bmp = New-Object System.Drawing.Bitmap(128, 128, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
    $g = [System.Drawing.Graphics]::FromImage($bmp)
    $g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias

    # Ribbon
    $ribbonBrush = New-Object System.Drawing.Drawing2D.LinearGradientBrush(
        (New-Object System.Drawing.Point(40, 0)),
        (New-Object System.Drawing.Point(88, 0)),
        $ribbonColor,
        [System.Drawing.Color]::FromArgb(255, [int]($ribbonColor.R * 0.7), [int]($ribbonColor.G * 0.7), [int]($ribbonColor.B * 0.7))
    )
    $ribbonPolyL = @(
        (New-Object System.Drawing.Point(46, 0)),
        (New-Object System.Drawing.Point(64, 52)),
        (New-Object System.Drawing.Point(50, 52)),
        (New-Object System.Drawing.Point(34, 0))
    )
    $ribbonPolyR = @(
        (New-Object System.Drawing.Point(82, 0)),
        (New-Object System.Drawing.Point(64, 52)),
        (New-Object System.Drawing.Point(78, 52)),
        (New-Object System.Drawing.Point(94, 0))
    )
    $g.FillPolygon($ribbonBrush, $ribbonPolyL)
    $g.FillPolygon($ribbonBrush, $ribbonPolyR)
    $ribbonBrush.Dispose()

    # Outer Disc
    $discBrush = New-Object System.Drawing.Drawing2D.LinearGradientBrush(
        (New-Object System.Drawing.Point(24, 38)),
        (New-Object System.Drawing.Point(104, 118)),
        $color1,
        $color2
    )
    $g.FillEllipse($discBrush, 24, 38, 80, 80)
    $discBrush.Dispose()

    # Outer Rim
    $rimPen = New-Object System.Drawing.Pen($rimColor, 4)
    $g.DrawEllipse($rimPen, 26, 40, 76, 76)
    $rimPen.Dispose()

    # Inner Ring
    $inRimPen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(180, 255, 255, 255), 1.5)
    $g.DrawEllipse($inRimPen, 32, 46, 64, 64)
    $inRimPen.Dispose()

    # Center Star
    $starBrush = New-Object System.Drawing.Drawing2D.LinearGradientBrush(
        (New-Object System.Drawing.Point(48, 62)),
        (New-Object System.Drawing.Point(80, 94)),
        [System.Drawing.Color]::FromArgb(255, 255, 255, 240),
        $color2
    )
    $starPts = @(
        (New-Object System.Drawing.Point(64, 60)),
        (New-Object System.Drawing.Point(68, 72)),
        (New-Object System.Drawing.Point(80, 72)),
        (New-Object System.Drawing.Point(71, 80)),
        (New-Object System.Drawing.Point(74, 92)),
        (New-Object System.Drawing.Point(64, 84)),
        (New-Object System.Drawing.Point(54, 92)),
        (New-Object System.Drawing.Point(57, 80)),
        (New-Object System.Drawing.Point(48, 72)),
        (New-Object System.Drawing.Point(60, 72))
    )
    $g.FillPolygon($starBrush, $starPts)
    $starBrush.Dispose()

    $g.Dispose()
    $bmp.Save("E:\lab2\Assets\$fileName", [System.Drawing.Imaging.ImageFormat]::Png)
    $bmp.Dispose()
    Write-Output "Generated $fileName"
}

Make-Medal ([System.Drawing.Color]::FromArgb(255, 255, 220, 80)) ([System.Drawing.Color]::FromArgb(255, 205, 145, 20)) ([System.Drawing.Color]::FromArgb(255, 255, 240, 160)) ([System.Drawing.Color]::FromArgb(255, 200, 30, 40)) "ui_medal_gold.png"
Make-Medal ([System.Drawing.Color]::FromArgb(255, 235, 242, 250)) ([System.Drawing.Color]::FromArgb(255, 140, 160, 185)) ([System.Drawing.Color]::FromArgb(255, 255, 255, 255)) ([System.Drawing.Color]::FromArgb(255, 35, 80, 180)) "ui_medal_silver.png"
Make-Medal ([System.Drawing.Color]::FromArgb(255, 215, 145, 90)) ([System.Drawing.Color]::FromArgb(255, 130, 75, 35)) ([System.Drawing.Color]::FromArgb(255, 245, 180, 130)) ([System.Drawing.Color]::FromArgb(255, 160, 45, 30)) "ui_medal_bronze.png"

# 3. Generate ui_arc_reactor.png (128x128 32-bit RGBA)
$arcBmp = New-Object System.Drawing.Bitmap(128, 128, [System.Drawing.Imaging.PixelFormat]::Format32bppArgb)
$g = [System.Drawing.Graphics]::FromImage($arcBmp)
$g.SmoothingMode = [System.Drawing.Drawing2D.SmoothingMode]::AntiAlias

# Outer Cyan Glow
$pathG = New-Object System.Drawing.Drawing2D.GraphicsPath
$pathG.AddEllipse(10, 10, 108, 108)
$glowBrush = New-Object System.Drawing.Drawing2D.PathGradientBrush($pathG)
$glowBrush.CenterColor = [System.Drawing.Color]::FromArgb(200, 0, 230, 255)
$glowBrush.SurroundColors = @([System.Drawing.Color]::FromArgb(0, 0, 50, 120))
$g.FillPath($glowBrush, $pathG)
$glowBrush.Dispose()
$pathG.Dispose()

# Outer Metallic Titanium Ring
$outerPen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(255, 255, 204, 50), 4.5)
$g.DrawEllipse($outerPen, 20, 20, 88, 88)
$outerPen.Dispose()

# Cyan Ring
$cyanPen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(255, 0, 225, 255), 3.0)
$g.DrawEllipse($cyanPen, 26, 26, 76, 76)
$cyanPen.Dispose()

# Dark Core
$coreBrush = New-Object System.Drawing.SolidBrush([System.Drawing.Color]::FromArgb(255, 14, 20, 32))
$g.FillEllipse($coreBrush, 30, 30, 68, 68)
$coreBrush.Dispose()

# 8 Glowing Power Coils
$coilBrush = New-Object System.Drawing.SolidBrush([System.Drawing.Color]::FromArgb(255, 0, 240, 255))
$coilGold = New-Object System.Drawing.SolidBrush([System.Drawing.Color]::FromArgb(255, 255, 215, 60))
for ($i = 0; $i -lt 8; $i++) {
    $angle = $i * [Math]::PI / 4.0
    $cx = 64 + [Math]::Cos($angle) * 38
    $cy = 64 + [Math]::Sin($angle) * 38
    $g.FillEllipse($coilBrush, [float]($cx - 5), [float]($cy - 5), 10.0, 10.0)
    $g.FillEllipse($coilGold, [float]($cx - 2), [float]($cy - 2), 4.0, 4.0)
}
$coilBrush.Dispose()
$coilGold.Dispose()

# Inner Glowing Core
$inPath = New-Object System.Drawing.Drawing2D.GraphicsPath
$inPath.AddEllipse(44, 44, 40, 40)
$inBrush = New-Object System.Drawing.Drawing2D.PathGradientBrush($inPath)
$inBrush.CenterColor = [System.Drawing.Color]::FromArgb(255, 255, 255, 255)
$inBrush.SurroundColors = @([System.Drawing.Color]::FromArgb(255, 0, 210, 255))
$g.FillPath($inBrush, $inPath)
$inBrush.Dispose()
$inPath.Dispose()

$centerPen = New-Object System.Drawing.Pen([System.Drawing.Color]::FromArgb(255, 0, 235, 255), 2.5)
$g.DrawEllipse($centerPen, 44, 44, 40, 40)
$centerPen.Dispose()

$g.Dispose()
$arcBmp.Save("E:\lab2\Assets\ui_arc_reactor.png", [System.Drawing.Imaging.ImageFormat]::Png)
$arcBmp.Dispose()
Write-Output "Generated ui_arc_reactor.png"
