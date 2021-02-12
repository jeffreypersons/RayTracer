#pragma once
#include <iostream>
#include "Math.hpp"


// assumes each color component in range[0.00, 1.00]
// note that brightness or alpha is not (currently) supported
struct Color {
private:
    static constexpr int MIN_8BIT = 0;
    static constexpr int MAX_8BIT = 255;
    static constexpr float INV_MAX_8BIT = 1.0f / MAX_8BIT;
    
    static constexpr float intToFloat(int v)                          { return v * INV_MAX_8BIT;                               }
    static constexpr int   floatToInt(float v)                        { return static_cast<int>(v * MAX_8BIT);                 }
    static constexpr unsigned char extractRedByte(unsigned int hex)   { return static_cast<unsigned char>((hex >> 16) & 0xff); }
    static constexpr unsigned char extractGreenByte(unsigned int hex) { return static_cast<unsigned char>((hex >> 8)  & 0xff); }
    static constexpr unsigned char extractBlueByte(unsigned int hex)  { return static_cast<unsigned char>(hex & 0xff);         }

public:
    float r;
    float g;
    float b;
    
    Color() = default;
    constexpr Color(float  r, float  g, float  b) : r(Math::clamp01(r)),       g(Math::clamp01(g)),       b(Math::clamp01(b))       {}
    constexpr Color(double r, double g, double b) : r(static_cast<float>(r)),  g(static_cast<float>(g)),  b(static_cast<float>(b))  {}
    constexpr Color(int    r, int    g, int    b) : r(intToFloat(r)),          g(intToFloat(g)),          b(intToFloat(b))          {}
    constexpr Color(int hex)                      : r(extractRedByte(hex)),    g(extractGreenByte(hex)),  b(extractBlueByte(hex))   {}
    constexpr Color(const Color& intensity)           : r(Math::clamp01(intensity.r)), g(Math::clamp01(intensity.g)), b(Math::clamp01(intensity.b)) {}

    constexpr unsigned int getHex() {
        return ( (0xff & static_cast<unsigned char>(floatToInt(r))) << 16 ) |
               ( (0xff & static_cast<unsigned char>(floatToInt(g))) << 8  ) |
               ( (0xff & static_cast<unsigned char>(floatToInt(b)))       );
    }
};
inline constexpr Color operator+(const Color& lhs, const Color& rhs)  { return Color(lhs.r + rhs.r, lhs.g + rhs.g, lhs.b + rhs.b);     }
inline constexpr Color operator-(const Color& lhs, const Color& rhs)  { return Color(lhs.r - rhs.r, lhs.g - rhs.g, lhs.b - rhs.b);     }
inline constexpr Color operator*(const Color& lhs, const Color& rhs)  { return Color(lhs.r * rhs.r, lhs.g * rhs.g, lhs.b * rhs.b);     }
inline constexpr Color operator*(float        lhs, const Color& rhs)  { return Color(lhs   * rhs.r, lhs   * rhs.g, lhs   * rhs.b);     }
inline constexpr Color operator*(const Color& lhs, float        rhs)  { return Color(lhs.r * rhs,   lhs.g * rhs,   lhs.b * rhs);       }
inline constexpr Color operator/(const Color& lhs, float        rhs)  { return Color(lhs.r / rhs,   lhs.g / rhs,   lhs.b / rhs);       }
inline std::ostream& operator<<(std::ostream& os, const Color& intensity) { os << intensity.r << "," << intensity.g << "," << intensity.b;  return os; }


namespace Palette {
// base colors
constexpr Color white   { 1.00, 1.00, 1.00 };
constexpr Color red     { 1.00, 0.00, 0.00 };
constexpr Color green   { 0.00, 1.00, 0.00 };
constexpr Color blue    { 0.00, 0.00, 1.00 };
constexpr Color cyan    { 0.00, 1.00, 1.00 };
constexpr Color magenta { 1.00, 0.00, 1.00 };
constexpr Color yellow  { 1.00, 1.00, 0.00 };
constexpr Color black   { 0.00, 0.00, 0.00 };

// assorted colors tediously matching online color guides..
constexpr Color aliceBlue            { 0.941176, 0.972549, 1.000000 };
constexpr Color antiqueWhite         { 0.980392, 0.921569, 0.843137 };
constexpr Color aqua                 { 0.000000, 0.000000, 1.000000 };
constexpr Color aquamarine           { 0.498039, 1.000000, 0.831373 };
constexpr Color azure                { 0.941176, 1.000000, 1.000000 };
constexpr Color beige                { 0.960784, 0.960784, 0.862745 };
constexpr Color bisque               { 1.000000, 0.894118, 0.768627 };
constexpr Color blanchedAlmond       { 1.000000, 0.921569, 0.803922 };
constexpr Color blueViolet           { 0.541176, 0.168627, 0.886275 };
constexpr Color brown                { 0.647059, 0.164706, 0.164706 };
constexpr Color burlyWood            { 0.870588, 0.721569, 0.529412 };
constexpr Color cadetBlue            { 0.372549, 0.619608, 0.627451 };
constexpr Color chartreuse           { 0.498039, 1.000000, 0.000000 };
constexpr Color chocolate            { 0.823529, 0.411765, 0.117647 };
constexpr Color coral                { 1.000000, 0.498039, 0.313726 };
constexpr Color cornflowerBlue       { 0.392157, 0.584314, 0.929412 };
constexpr Color cornsilk             { 1.000000, 0.972549, 0.862745 };
constexpr Color crimson              { 0.862745, 0.078431, 0.235294 };
constexpr Color darkBlue             { 0.000000, 0.000000, 0.545098 };
constexpr Color darkCyan             { 0.000000, 0.545098, 0.545098 };
constexpr Color darkGray             { 0.662745, 0.662745, 0.662745 };
constexpr Color darkGreen            { 0.000000, 0.392157, 0.000000 };
constexpr Color darkKhaki            { 0.741176, 0.717647, 0.419608 };
constexpr Color darkMagenta          { 0.545098, 0.000000, 0.545098 };
constexpr Color darkOliveGreen       { 0.333333, 0.419608, 0.184314 };
constexpr Color darkorange           { 1.000000, 0.549020, 0.000000 };
constexpr Color darkOrchid           { 0.600000, 0.196078, 0.800000 };
constexpr Color darkRed              { 0.545098, 0.000000, 0.000000 };
constexpr Color darkSalmon           { 0.913725, 0.588235, 0.478431 };
constexpr Color darkSeaGreen         { 0.560784, 0.737255, 0.560784 };
constexpr Color darkSlateBlue        { 0.282353, 0.239216, 0.545098 };
constexpr Color darkSlateGray        { 0.184314, 0.309804, 0.309804 };
constexpr Color darkTurquoise        { 0.000000, 0.807843, 0.819608 };
constexpr Color darkViolet           { 0.580392, 0.000000, 0.827451 };
constexpr Color deepPink             { 1.000000, 0.078431, 0.576471 };
constexpr Color deepSkyBlue          { 0.000000, 0.749020, 1.000000 };
constexpr Color dimGray              { 0.411765, 0.411765, 0.411765 };
constexpr Color dodgerBlue           { 0.117647, 0.564706, 1.000000 };
constexpr Color fireBrick            { 0.698039, 0.133333, 0.133333 };
constexpr Color floralWhite          { 1.000000, 0.980392, 0.941176 };
constexpr Color forestGreen          { 0.133333, 0.545098, 0.133333 };
constexpr Color fuchsia              { 1.000000, 0.000000, 1.000000 };
constexpr Color gainsboro            { 0.862745, 0.862745, 0.862745 };
constexpr Color ghostWhite           { 0.972549, 0.972549, 1.000000 };
constexpr Color gold                 { 1.000000, 0.843137, 0.000000 };
constexpr Color goldenRod            { 0.854902, 0.647059, 0.125490 };
constexpr Color gray                 { 0.501961, 0.501961, 0.501961 };
constexpr Color greenYellow          { 0.678431, 1.000000, 0.184314 };
constexpr Color honeyDew             { 0.941176, 1.000000, 0.941176 };
constexpr Color hotPink              { 1.000000, 0.411765, 0.705882 };
constexpr Color indianRed            { 0.803922, 0.360784, 0.360784 };
constexpr Color indigo               { 0.294118, 0.000000, 0.509804 };
constexpr Color ivory                { 1.000000, 1.000000, 0.941176 };
constexpr Color khaki                { 0.941176, 0.901961, 0.549020 };
constexpr Color lavender             { 0.901961, 0.901961, 0.980392 };
constexpr Color lavenderBlush        { 1.000000, 0.941176, 0.960784 };
constexpr Color lawnGreen            { 0.486275, 0.988235, 0.000000 };
constexpr Color lemonChiffon         { 1.000000, 0.980392, 0.803922 };
constexpr Color lightBlue            { 0.678431, 0.847059, 0.901961 };
constexpr Color lightCoral           { 0.941176, 0.501961, 0.501961 };
constexpr Color lightCyan            { 0.878431, 1.000000, 1.000000 };
constexpr Color lightGoldenRodYellow { 0.980392, 0.980392, 0.823529 };
constexpr Color lightGray            { 0.827451, 0.827451, 0.827451 };
constexpr Color lightGreen           { 0.564706, 0.933333, 0.564706 };
constexpr Color lightPink            { 1.000000, 0.713726, 0.756863 };
constexpr Color lightSalmon          { 1.000000, 0.627451, 0.478431 };
constexpr Color lightSeaGreen        { 0.125490, 0.698039, 0.666667 };
constexpr Color lightSkyBlue         { 0.529412, 0.807843, 0.980392 };
constexpr Color lightSlateGray       { 0.466667, 0.533333, 0.600000 };
constexpr Color lightSteelBlue       { 0.690196, 0.768627, 0.870588 };
constexpr Color lightYellow          { 1.000000, 1.000000, 0.878431 };
constexpr Color lime                 { 0.000000, 1.000000, 0.000000 };
constexpr Color limeGreen            { 0.196078, 0.803922, 0.196078 };
constexpr Color linen                { 0.980392, 0.941176, 0.901961 };
constexpr Color maroon               { 0.501961, 0.000000, 0.000000 };
constexpr Color mediumAquaMarine     { 0.400000, 0.803922, 0.666667 };
constexpr Color mediumBlue           { 0.000000, 0.000000, 0.803922 };
constexpr Color mediumOrchid         { 0.729412, 0.333333, 0.827451 };
constexpr Color mediumPurple         { 0.576471, 0.439216, 0.858824 };
constexpr Color mediumSeaGreen       { 0.235294, 0.701961, 0.443137 };
constexpr Color mediumSlateBlue      { 0.482353, 0.407843, 0.933333 };
constexpr Color mediumSpringGreen    { 0.000000, 0.980392, 0.603922 };
constexpr Color mediumTurquoise      { 0.282353, 0.819608, 0.800000 };
constexpr Color mediumVioletRed      { 0.780392, 0.082353, 0.521569 };
constexpr Color midnightBlue         { 0.098039, 0.098039, 0.439216 };
constexpr Color mintCream            { 0.960784, 1.000000, 0.980392 };
constexpr Color mistyRose            { 1.000000, 0.894118, 0.882353 };
constexpr Color moccasin             { 1.000000, 0.894118, 0.709804 };
constexpr Color navajoWhite          { 1.000000, 0.870588, 0.678431 };
constexpr Color navy                 { 0.000000, 0.000000, 0.501961 };
constexpr Color oldLace              { 0.992157, 0.960784, 0.901961 };
constexpr Color olive                { 0.501961, 0.501961, 0.000000 };
constexpr Color oliveDrab            { 0.419608, 0.556863, 0.137255 };
constexpr Color orange               { 1.000000, 0.647059, 0.000000 };
constexpr Color orangeRed            { 1.000000, 0.270588, 0.000000 };
constexpr Color orchid               { 0.854902, 0.439216, 0.839216 };
constexpr Color paleGoldenRod        { 0.933333, 0.909804, 0.666667 };
constexpr Color paleGreen            { 0.596078, 0.984314, 0.596078 };
constexpr Color paleTurquoise        { 0.686275, 0.933333, 0.933333 };
constexpr Color paleVioletRed        { 0.858824, 0.439216, 0.576471 };
constexpr Color papayaWhip           { 1.000000, 0.937255, 0.835294 };
constexpr Color peachPuff            { 1.000000, 0.854902, 0.725490 };
constexpr Color peru                 { 0.803922, 0.521569, 0.247059 };
constexpr Color pink                 { 1.000000, 0.752941, 0.796078 };
constexpr Color plum                 { 0.866667, 0.627451, 0.866667 };
constexpr Color powderBlue           { 0.690196, 0.878431, 0.901961 };
constexpr Color purple               { 0.501961, 0.000000, 0.501961 };
constexpr Color rosyBrown            { 0.737255, 0.560784, 0.560784 };
constexpr Color royalBlue            { 0.254902, 0.411765, 0.882353 };
constexpr Color saddleBrown          { 0.545098, 0.270588, 0.074510 };
constexpr Color salmon               { 0.980392, 0.501961, 0.447059 };
constexpr Color sandyBrown           { 0.956863, 0.643137, 0.376471 };
constexpr Color seaGreen             { 0.180392, 0.545098, 0.341176 };
constexpr Color seaShell             { 1.000000, 0.960784, 0.933333 };
constexpr Color sienna               { 0.627451, 0.321569, 0.176471 };
constexpr Color silver               { 0.752941, 0.752941, 0.752941 };
constexpr Color skyBlue              { 0.529412, 0.807843, 0.921569 };
constexpr Color slateBlue            { 0.415686, 0.352941, 0.803922 };
constexpr Color slateGray            { 0.439216, 0.501961, 0.564706 };
constexpr Color snow                 { 1.000000, 0.980392, 0.980392 };
constexpr Color springGreen          { 0.000000, 1.000000, 0.498039 };
constexpr Color steelBlue            { 0.274510, 0.509804, 0.705882 };
constexpr Color blueSteel            { 0.274510, 0.509804, 0.705882 };
constexpr Color tan                  { 0.823529, 0.705882, 0.549020 };
constexpr Color teal                 { 0.000000, 0.501961, 0.501961 };
constexpr Color thistle              { 0.847059, 0.749020, 0.847059 };
constexpr Color tomato               { 1.000000, 0.388235, 0.278431 };
constexpr Color turquoise            { 0.250980, 0.878431, 0.815686 };
constexpr Color violet               { 0.933333, 0.509804, 0.933333 };
constexpr Color wheat                { 0.960784, 0.870588, 0.701961 };
constexpr Color whiteSmoke           { 0.960784, 0.960784, 0.960784 };
constexpr Color yellowGreen          { 0.603922, 0.803922, 0.196078 };
}
