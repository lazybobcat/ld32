#include <entities/textnode.h>
#include <utils.h>

TextNode::TextNode(const std::string& text, const FontHolder& fonts, int fz) :
    mText(text, fonts.get(Fonts::Main), fz)
{
    centerOrigin(mText);
}

void TextNode::setText(const std::string &text)
{
    mText.setString(text);
    centerOrigin(mText);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
    target.draw(mText, states);
}
