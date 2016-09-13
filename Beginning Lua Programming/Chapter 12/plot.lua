require "gd"

local Cn = {
  -- FontStr = "c:/windows/fonts/arial.ttf",
  FontStr = "vera",
  Height = 1024,
  Margin = 12,
  PointSize = 36,
}

local function ImgInit()
  local Img = {}
  Img.Height = Cn.Height
  Img.Width = Img.Height * (1 + math.sqrt(5)) / 2
  Img.Hnd = gd.createTrueColor(Img.Width, Img.Height)
  Img.White = Img.Hnd:colorAllocate(255, 255, 255)
  Img.Black = Img.Hnd:colorAllocate(0, 0, 0)
  Img.Gray = Img.Hnd:colorAllocate(192, 192, 192)
  local Ext = {gd.stringFT(nil, Img.White,
    Cn.FontStr, Cn.PointSize, 0, Cn.Margin, Cn.Margin, "X")}
  Img.ChHt = Ext[2] - Ext[8]
  Img.Left = Cn.Margin
  Img.Right = Img.Width - Img.Left - 1
  Img.Top = Cn.Margin + Img.ChHt + Cn.Margin
  Img.Bottom = Img.Height - Img.Left - 1
  Img.Hnd:setAntiAliased(Img.Black)
  Img.Hnd:filledRectangle(0, 0, Img.Width - 1, Img.Height - 1, Img.White)
  Img.Hnd:rectangle(Img.Left, Img.Top, Img.Right, Img.Bottom, Img.Black)
  return Img
end

-- Write antialiased text. XAlign is "left", "center", or "right" and qualifies
-- X. YAlign is "top", "middle", or "bottom" and qualifies Y.

local function LclWrite(Img, X, XAlign, Y, YAlign, Clr, TextStr)
  local Lf, Bt, Rt, _, _, Tp = gd.stringFT(nil, Clr, Cn.FontStr,
    Cn.PointSize, 0, 0, 0, TextStr)
  local Wd = Rt - Lf
  X = XAlign == "center" and X - Wd / 2 or XAlign == "right" and X - Wd or X
  Y = YAlign == "middle" and Y + Img.ChHt / 2 or
    YAlign == "top" and Y + Img.ChHt or Y
  Img.Hnd:stringFT(Clr, Cn.FontStr, Cn.PointSize, 0, X, Y, TextStr)
end

-- Plot Y = Fnc(X) from X1 to X2. Disjoint is true if change in Y lifts pen.

local function Plot(Img, X1, Y1, X2, Y2, Fnc, TitleStr, Disjoint)
  -- Mapping functions
  -- X = Xm * H + Xb where H is horizontal pixel unit
  -- H = Hm * X + Hb
  -- Y = Fnc(X)
  -- V = Vm * Y + Vb where V is vertical pixel unit
  local Lift, X, Xm, Xb, V, Vm, Vb, Y, H, HPrv, VPrv, Ht, Wd,
    Hm, Hb, YPrv, BadVal
  Ht = Img.Bottom - Img.Top
  Wd = Img.Right - Img.Left
  Xm = (X2 - X1) / Wd
  Xb = X2 - Xm * Img.Right
  Hm = Wd / (X2 - X1)
  Hb = Img.Left - Hm * X1
  Vm = Ht / (Y1 - Y2)
  Vb = Img.Bottom - Vm * Y1

  LclWrite(Img, Cn.Margin, "left", Cn.Margin, "top", Img.Black, TitleStr)
  Img.Hnd:setClip(Img.Left, Img.Top, Img.Right, Img.Bottom)
  Img.Hnd:line(Img.Left + 1, Vb, Img.Right - 1, Vb, Img.Gray) -- Y = 0
  Img.Hnd:line(Hb, Img.Top + 1, Hb, Img.Bottom - 1, Img.Gray) -- X = 0
  BadVal = tostring(math.asin(2))
  Lift = true
  for H = Img.Left, Img.Right do
    X = Xm * H + Xb
    X = X < X1 and X1 or X > X2 and X2 or X -- Constrain sign at boundaries
    Y = Fnc(X)
    if tostring(Y) ~= BadVal then
      if Disjoint and Y ~= YPrv then
        Lift = true
      end
      YPrv = Y
      V = Vm * Y + Vb
      if Lift then
        Lift = false
      else
        if Y >= Y1 and Y <= Y2 then
          Img.Hnd:line(HPrv, VPrv, H, V, Img.Hnd.ANTI_ALIASED)
        end
      end
      VPrv = V
      HPrv = H
    end
  end
  LclWrite(Img, Img.Left + Cn.Margin, "left", Img.Bottom - Cn.Margin, "bottom",
    Img.Gray, "(" .. X1 .. ", " .. Y1 .. ")")
  LclWrite(Img, Img.Right - Cn.Margin, "right", Img.Top + Cn.Margin, "top",
    Img.Gray, "(" .. X2 .. ", " .. Y2 .. ")")
end

function Waveform(X)
  return math.sin(3*X) * math.sin(X/3)
end

local Img = ImgInit()
Plot(Img, -12, -1.2,  12, 1.2, Waveform, "Waveform")
Img.Hnd:png("waveform.png")
