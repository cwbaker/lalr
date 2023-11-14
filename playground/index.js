// Setup editors
function setupInfoArea(id) {
  const e = ace.edit(id);
  e.setShowPrintMargin(false);
  e.setOptions({
    readOnly: true,
    highlightActiveLine: false,
    highlightGutterLine: false
  })
  e.renderer.$cursorLayer.element.style.opacity=0;
  return e;
}

function setupEditorArea(id, lsKey) {
  const e = ace.edit(id);
  e.setShowPrintMargin(false);
  e.setValue(localStorage.getItem(lsKey) || '');
  e.moveCursorTo(0, 0);
  return e;
}

let userContentHasChanged = false;
let grammarContentHasChanged = false;
let inputContentHasChanged = false;
function grammarOnChange(delta) {
	if(!grammarContentHasChanged) {
		grammarContentHasChanged = true;
		userContentHasChanged = true;
	}
}
function inputOnChange(delta) {
	if(!inputContentHasChanged) {
		inputContentHasChanged = true;
		userContentHasChanged = true;
	}
}

const grammarEditor = setupEditorArea("grammar-editor", "grammarText");
grammarEditor.on("change", grammarOnChange);
grammarEditor.getSession().setMode("ace/mode/yaml");
const codeEditor = setupEditorArea("code-editor", "codeText");
codeEditor.on("change", inputOnChange);
userContentHasChanged = localStorage.getItem("userContentHasChanged");

const codeAst = setupInfoArea("code-ast");
const codeLexer = setupInfoArea("code-lexer");

const sampleList = [
	//title, grammar, input, input ace syntax
	["XML parser", "xml.g", "test.xml.txt", "ace/mode/xml"],
	["LALR parser", "lalr.g", "lalr.g", "ace/mode/yaml"],
	["Calculator error handling parser", "error_handling_calculator.g", "error_handling_calculator.txt", "ace/mode/text"],
	["Json parser", "json3.g", "test.json.txt", "ace/mode/json"],
	["Json5 parser", "json5.g", "test.json5.txt", "ace/mode/json"],
	["Lua parser", "lua.g", "test.lua", "ace/mode/lua"],
	["JavascriptCore parser", "javascript-core.g", "test.js.txt", "ace/mode/javascript"],
	["Carbon parser", "carbon-lang.g", "prelude.carbon", "ace/mode/typescript"],
	["Linden Script parser", "lsl_ext.g", "test.lsl", "ace/mode/text"],
	["Ansi C parser", "cparser.g", "test.c", "ace/mode/c_cpp"],
	["Ispc parser", "ispc.g", "test.ispc", "ace/mode/c_cpp"],
	["Java11 parser", "java11.g", "test.java", "ace/mode/java"],
	["Rust parser", "rust.g", "test.rs", "ace/mode/rust"],
	["CQL parser (be patient)", "cql.g", "test.cql", "ace/mode/sql"],
	["Postgresql parser (be patient)", "postgresql-16.g", "test.sql", "ace/mode/sql"],
	["Postgresql parser case insensitive (be patient)", "postgresql-16-nc.g", "test.sql", "ace/mode/sql"],
	["Gringo/Clingo non grounded parser", "gringo-ng.g", "test.clingo", "ace/mode/text"],
	["Ada parser", "ada-adayacc.g", "test.adb", "ace/mode/ada"],
	["Ada parser case insensitive", "ada-adayacc-n.g", "test.adb", "ace/mode/ada"],
	["Textmapper parser", "textmapper.g", "test.tm", "ace/mode/yaml"],
	["C++ parser (bug)", "cxx-parser.g", "test.cpp", "ace/mode/c_cpp"],
	["Bison parser (not working)", "bison.g", "carbon-lang.y", "ace/mode/yaml"],
	["DParser parser (not working)", "dparser.g", "test.dparser", "ace/mode/yaml"],
	["Parse_gen parser (not working)", "parse_gen.g", "test.parse_gen", "ace/mode/yaml"],
	["Tameparse parser (not working)", "tameparser.g", "test.tameparser", "ace/mode/yaml"],
	["Javascript parser (not working)", "javascript.g", "test.js.txt", "ace/mode/javascript"],
	["Go parser (not working)", "go.g", "test.go", "ace/mode/golang"],
	["PHP-8.2 parser (not working)", "php-8.2.g", "test.php", "ace/mode/php"],
	["BC calculator parser", "bc.g", "test.bc", "ace/mode/text"],
	["CFront3 parser (notworking)", "cfront3.g", "test.cpp", "ace/mode/c_cpp"],
	["C++98 parser (notworking)", "cpp-98-parser.g", "test.cpp", "ace/mode/c_cpp"],
	["Cyclone parser (notworking)", "cyclone.g", "test.c", "ace/mode/c_cpp"],
	["Cobalt CSS parser (notworking)", "cobalt-css.g", "style.css", "ace/mode/css"],
	["Flex parser (notworking)", "flex-parser.g", "test.flex", "ace/mode/yaml"],
	["Frege parser (notworking)", "frege-parser.g", "test.c", "ace/mode/c_cpp"],
	["GHC parser (notworking)", "ghc-parser.g", "test.c", "ace/mode/c_cpp"],
	["Graphql-ruby parser (be patient)", "graphql-ruby.g", "test.cql", "ace/mode/sql"],
	["HTML parser (be patient)", "html-parser.g", "index.html", "ace/mode/html"],
	["HTTPD parser (be patient)", "httpd-parser.g", "index.html", "ace/mode/html"],
	["IVerilog parser (be patient)", "iverilog-parser.g", "test.vl", "ace/mode/verilog"],
];

function loadLalr_sample(self) {
  if(userContentHasChanged)
  {
	let ok = confirm("Your changes will be lost !\nIf the changes you've made are important save then before proceed.\nCopy and paste to your prefered editor and save it.\nEither OK or Cancel.");
	if(!ok) return false;
  }
  let base_url = "./"
  if(self.selectedIndex > 0) {
      let sample_to_use = sampleList[self.selectedIndex-1];
      $.get(base_url + sample_to_use[1], function( data ) {
        grammarEditor.setValue( data );
	grammarContentHasChanged = false;
	userContentHasChanged = false;
      });
      $.get(base_url + sample_to_use[2], function( data ) {
        codeEditor.setValue( data );
	codeEditor.getSession().setMode(sample_to_use[3]);
	inputContentHasChanged = false;
	userContentHasChanged = false;
      });
  }
}

//$('#ast-mode').val(localStorage.getItem('optimizationMode') || '2');
$('#auto-refresh').prop('checked', localStorage.getItem('autoRefresh') === 'true');
$('#parse').prop('disabled', $('#auto-refresh').prop('checked'));

// Parse
function escapeHtml(unsafe) {
  return unsafe
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;")
    .replace(/"/g, "&quot;")
    .replace(/'/g, "&#039;");
}

function nl2br(str) {
  return str.replace(/\n/g, '<br>\n')
}

function textToErrors(str) {
  let errors = [];
  var regExp = /([^\n]+?)\n/g, match;
  while (match = regExp.exec(str)) {
    let msg = match[1];
    let line_col = msg.match(/lalr \((\d+):(\d+)\):/);
    if (line_col) {
      errors.push({"ln": line_col[1], "col":line_col[2], "msg": msg});
    } else {
      errors.push({"msg": msg});
    }
  }
  return errors;
}

function generateErrorListHTML(errors) {
  let html = '<ul>';

  html += $.map(errors, function (x) {
    if (x.ln > 0) {
      return '<li data-ln="' + x.ln + '" data-col="' + x.col +
        '"><span>' + escapeHtml(x.msg) + '</span></li>';
    } else {
      return '<li><span>' + escapeHtml(x.msg) + '</span></li>';
    }
  }).join('');

  html += '<ul>';

  return html;
}

function updateLocalStorage() {
  if(grammarContentHasChanged || inputContentHasChanged)
  {
    localStorage.setItem('grammarText', grammarEditor.getValue());
    localStorage.setItem('codeText', codeEditor.getValue());
    grammarContentHasChanged = false;
    inputContentHasChanged = false;
    localStorage.setItem('userContentHasChanged', userContentHasChanged);
  }
  //localStorage.setItem('optimizationMode', $('#opt-mode').val());
  //localStorage.setItem('autoRefresh', $('#auto-refresh').prop('checked'));
}

var parse_start_time = 0;

function parse() {
  const $grammarValidation = $('#grammar-validation');
  const $grammarInfo = $('#grammar-info');
  const grammarText = grammarEditor.getValue();

  const $codeValidation = $('#code-validation');
  const $codeInfo = $('#code-info');
  const codeText = codeEditor.getValue();

  const astMode = $('#ast-mode').val();
  const generate_ebnf = $('#generate-action').val() == 'ebnf';
  const generate_yacc = $('#generate-action').val() == 'yacc';
  const generate_yacc_html = $('#generate-action').val() == 'yacc_html';
  const lexer = $('#show-lexer').prop('checked');
  let generate_ast = $('#show-ast').prop('checked');
  if(generate_ast && astMode == 2)
	generate_ast = 2;

  $grammarInfo.html('');
  $grammarValidation.hide();
  $codeInfo.html('');
  $codeValidation.hide();
  codeAst.setValue('');
  codeLexer.setValue('');

  outputs.compile_status = '';
  outputs.parse_status = '';
  outputs.parse_stats = '';
  outputs.ast = '';
  outputs.lexer = '';
  outputs.parse_time = '';

  if (grammarText.length === 0) {
    return;
  }

  $('#overlay').css({
    'z-index': '1',
    'display': 'block',
    'background-color': 'rgba(0, 0, 0, 0.1)'
  });

  window.setTimeout(() => {
    parse_start_time = new Date().getTime();
    lalr_parse(grammarText, codeText, lexer, generate_ebnf, generate_yacc, generate_yacc_html, generate_ast);

    $('#overlay').css({
      'z-index': '-1',
      'display': 'none',
      'background-color': 'rgba(1, 1, 1, 1.0)'
    });

    const isGenEBNF = result.parse == -3;
    if (result.compile == 0 || isGenEBNF) {
      $grammarValidation.removeClass('validation-invalid').show();

      codeLexer.insert(outputs.lexer);

      if (result.parse == 0) {
        $codeValidation.removeClass('validation-invalid').show();
      } else {
        $codeValidation.addClass('validation-invalid').show();
      }

      if(isGenEBNF) {
	//if(generate_yacc_html)
	//{
	//	const win = window.open('', 'Yacc_HTML_linked');
	//	win.document.write(outputs.compile_status);
	//	win.document.close();
	//	win.focus();
	//}
	//else
		$grammarInfo.html("<pre>" + escapeHtml(outputs.compile_status.replaceAll("'\\''", '"\'"')) + "</pre>");
        return;
      }

      if (outputs.parse_status.length > 0) {
        const errors = textToErrors(outputs.parse_status);
        const html = generateErrorListHTML(errors);
        $codeInfo.html(html);
      }
      else {
        //const parse_end_time = new Date().getTime();
        //$codeInfo.html('Execution time: ' + (parse_end_time - parse_start_time) + ' ms');
        $codeInfo.html(outputs.parse_time);
	$grammarInfo.html("<pre>" + outputs.parse_stats + "</pre>");
      }
    } else {
      $grammarValidation.addClass('validation-invalid').show();
    }

    if (outputs.compile_status.length > 0) {
      const errors = textToErrors(outputs.compile_status);
      const html = generateErrorListHTML(errors);
      $grammarInfo.html(html);
    }

    if (outputs.ast.length > 0) {
      codeAst.setValue(outputs.ast);
    }
  }, 0);
}

// Event handing for text editing
let timer;
function setupTimer() {
  clearTimeout(timer);
  timer = setTimeout(() => {
    updateLocalStorage();
    if ($('#auto-refresh').prop('checked')) {
      parse();
    }
  }, 200);
};
grammarEditor.getSession().on('change', setupTimer);
codeEditor.getSession().on('change', setupTimer);

// Event handing in the info area
function makeOnClickInInfo(editor) {
  return function () {
    const el = $(this);
    let line = el.data('ln') - 1;
    let col = el.data('col') - 1;
    editor.navigateTo(line, col);
    editor.scrollToLine(line, true, false, null);
    editor.focus();
  }
};
$('#grammar-info').on('click', 'li[data-ln]', makeOnClickInInfo(grammarEditor));
$('#code-info').on('click', 'li[data-ln]', makeOnClickInInfo(codeEditor));

// Event handing in the AST optimization
$('#opt-mode').on('change', setupTimer);
$('#generate-action').on('change', setupTimer);
$('#show-lexer').on('change', setupTimer);
$('#auto-refresh').on('change', () => {
  updateLocalStorage();
  $('#parse').prop('disabled', $('#auto-refresh').prop('checked'));
  setupTimer();
});
$('#parse').on('click', parse);

// Resize editors to fit their parents
function resizeEditorsToParent() {
  codeEditor.resize();
  codeEditor.renderer.updateFull();
  codeAst.resize();
  codeAst.renderer.updateFull();
  codeLexer.resize();
  codeLexer.renderer.updateFull();
}

// Show windows
function setupToolWindow(lsKeyName, buttonSel, codeSel, showDefault) {
  let storedValue = localStorage.getItem(lsKeyName);
  if (!storedValue) {
    localStorage.setItem(lsKeyName, showDefault);
    storedValue = localStorage.getItem(lsKeyName);
  }
  let show = storedValue === 'true';
  $(buttonSel).prop('checked', show);
  $(codeSel).css({ 'display': show ? 'block' : 'none' });

  $(buttonSel).on('change', () => {
    show = !show;
    localStorage.setItem(lsKeyName, show);
    $(codeSel).css({ 'display': show ? 'block' : 'none' });
    resizeEditorsToParent();
  });
}

setupToolWindow('show-ast', '#show-ast', '#code-ast', true);
setupToolWindow('show-lexer', '#show-lexer', '#code-lexer', false);

// Show page
$('#main').css({
  'display': 'flex',
});

// used to collect output from C
var outputs = {
  'default': '',
  'compile_status': '',
  'ast': '',
  'parse_status': '',
  'parse_stats': '',
  'parse_time': '',
};

// current output (key in `outputs`)
var output = "default";

// results of the various stages
var result = {
  'compile': 0,
  'parse': 0,
  'ast': 0,
  'lexer': 0,
};

// lalr_parse function: initialized when emscripten runtime loads
var lalr_parse = null;

// Emscripten
var Module = {

  // intercept stdout (print) and stderr (printErr)
  // note: text received is line based and missing final '\n'

  'print': function(text) {
    outputs[output] += text + "\n";
  },
  'printErr': function(text) {
    outputs[output] += text + "\n";
  },

  // called when emscripten runtime is initialized
  'onRuntimeInitialized': function() {
    // wrap the C `parse` function
    lalr_parse = cwrap('parse', 'number', ['string', 'string', 'number']);
    // Initial parse
    if ($('#auto-refresh').prop('checked')) {
      parse();
    }
  },
};

function doFinalSettings() {
	let select_samples = document.getElementById('opt-samples');
	sampleList.map( (lang, i) => {
           let opt = document.createElement("option");
           opt.value = i; // the index
           opt.innerHTML = lang[0];
           select_samples.append(opt);
        });
}

// vim: sw=2:sts=2
