#!/usr/bin/env ruby
# -*- coding: utf-8 -*-

# TeX article file convertor

require 'optparse'

def parse_args(argv)
  opt = Hash.new
  parser = OptionParser.new

  parser.banner = "#{$0} [options] TEX_FILE"

  opt[:some_option] = 'default value'
  parser.on('-o', '--option VALUE') do |value|
    opt[:some_option] = value
  end

  parser.parse!(argv)

  if argv.size != 1
    puts parser.help
    exit 1
  end
  opt[:file] = argv.shift

  unless opt[:file] =~ /\.tex\Z/
    $stderr.puts "TeX file required: #{opt[:file]}"
    puts parser.help
    exit 1
  end

  opt
end

LBRACE = "!!!LBRACE!!!"
RBRACE = "!!!RBRACE!!!"


# document specific conversion rules

def do_adhoc(str)
  str.gsub!(/^\\cleardoublepage$/, "")
  str.gsub!(/^\\plainifnotempty$/, "")

  text_pairs = {
    %!$10^{12} = 1 \\mathrm{TB}$! => %!@<raw>#{LBRACE}|html|10<sup>12</sup>#{RBRACE}=1TB!,
  }

  text_pairs.each do |k,v|
    regex = Regexp.compile(Regexp.quote(k))
    str.gsub!(regex, v)
  end

  str
end

def main(argv)
  opt = parse_args(argv)

  tex_file = opt[:file]
  basename = File.basename(tex_file, ".tex")

  str = File.read(tex_file)

  # strip comment
  str.gsub!(/^%.*\n?/, "")
  str.gsub!(/((?!\\).)%.*\n?/, "\\1")

  str = do_adhoc(str)

  # noindent
  str.gsub!(/^\\noindent\s*/, "//noindent\n")

  # make chapter/section/subsection/paragraph an independent block
  str.gsub!(/(\\(?:chapter|(?:sub)*section|paragraph))\*?\{(.*)\}/) do |m|
    case $1
    when "\\chapter"
      prefix = "= "
    when "\\section"
      prefix = "== "
    when "\\subsection"
      prefix = "=== "
    when "\\subsubsection"
      prefix = "==== "
    when "\\paragraph"
      prefix = "===== "
    else
      raise RuntimeError.new
    end

    "\n" + prefix + $2 + "\n"
  end

  # remove lettrine
  str.gsub!(/\\lettrine\{(.+)\}\s*(?:(\\\s+)*)/, "\\1")

  # remove umlaut
  str.gsub!(/\\"{(.)}/) do |m|
    case $1
    when "o"
      "@<raw>#{LBRACE}|html|&ouml;#{RBRACE}"
    else
      raise RuntimeError.new
    end
  end

  # convert dash
  str.gsub!(/---/, "ー")
  str.gsub!(/--/, "-")

  # ruby
  str.gsub!(/\\ruby\{([^\}]+)\}\{([^\}]+)\}/) do |m|
    "@<ruby>{#{$1},#{$2}}"
  end

  # bf
  str.gsub!(/\{\\bf([^\}]*)\}/) do |m|
    "@<b>#{LBRACE}#{$1.strip}#{RBRACE}"
  end

  # it
  str.gsub!(/\{\\it([^\}]*)\}/) do |m|
    "@<i>#{LBRACE}#{$1.strip}#{RBRACE}"
  end

  # double quote
  str.gsub!(/``([^']*)''/) do |m|
    "@<raw>#{LBRACE}|html|&ldquo;#{RBRACE}" + $1 + "@<raw>#{LBRACE}|html|&rdquo;#{RBRACE}"
  end

  str.gsub!(/"([^"]*)"/) do |m|
    "@<raw>#{LBRACE}|html|&ldquo;#{RBRACE}" + $1 + "@<raw>#{LBRACE}|html|&rdquo;#{RBRACE}"
  end

  # remove escape of %/_/#
  str.gsub!(/\\(%|_|#)/, "\\1")

  # footnote
  blocks = str.split(/\n\n+/)

  fnidx = 0
  next_fnid = lambda { fnidx += 1; basename + "-fn-" + fnidx.to_s}

  blocks = blocks.map do |block|
    footnotes = []
    block = block.gsub(/\\footnote\{([^\}]*)\}/) do |m|
      fnid = next_fnid.call()
      fntxt = $1.gsub(/\n/, "")
      footnotes.push("//footnote[#{fnid}][#{fntxt}]")
      "@<fn>{#{fnid}}"
    end
    if footnotes.size > 0
      block + "\n" + footnotes.join("\n")
    else
      block
    end
  end

  # Ad-hoc conversion of author name
  (0..3).each do |idx|
    block = blocks[idx]
    if block =~ /\\begin\{flushright\}(?:\s|\n)*(?:\{\\headfont)?\s*([^\}\\]+)\}?(?:\s|\n)*\\end\{flushright\}/
      blocks[idx] = "//raw[|html|<div style=\"text-align: right\">#{$~[1].strip}</div>]"
    end
  end

  str = blocks.join("\n\n")

  # recover braces
  str.gsub!(LBRACE, "{")
  str.gsub!(RBRACE, "}")

  puts str
end

if __FILE__ == $0
  main(ARGV.dup)
end
