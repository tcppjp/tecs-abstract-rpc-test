# -*- coding: utf-8 -*-

require_tecsgen_lib "lib/GenOpaqueMarshaler.rb"
require_tecsgen_lib "lib/GenParamCopy.rb"

AbstractRPCProxyPluginArgProg = {
    # Specifies the name of client celltype to generate.
    "clientCelltypeName" => Proc.new { |obj, rhs| obj.set_client_celltype_name(rhs) },
    # Specifies the name of server celltype to generate.
    "serverCelltypeName" => Proc.new { |obj, rhs| obj.set_server_celltype_name(rhs) },
}

# The signature plugin for generating an Abstact Opaque RPC client/server proxy.
#
# Given a signature `sSignature`, this plugin generates a celltype like the
# following:
#
#   import(<rpc/sChannel.cdl>); /* etc. */
#
#   composite tSignatureClientProxy {
#       entry sSignature eEntry;
#       call sChannel cChannel;
#       /* ... */
#   }
#
#   composite tSignatureServerProxy {
#       call sSignature cCall;
#       call sChannel cChannel;
#       entry sUnmarshalerMain  eService;
#       /* ... */
#   }
#
class AbstractRPCProxyPlugin < SignaturePlugin
    def initialize(signature, option)
        super

        # Parse the plugin parameters
        @plugin_arg_check_proc_tab = AbstractRPCProxyPluginArgProg
        @plugin_arg_str = option
        @plugin_arg_str.sub!( /\A"(.*)/, '\1' )    # strip "" around the parameters
        @plugin_arg_str.sub!( /(.*)"\z/, '\1' )
        parse_plugin_arg

        unless @client_celltype_name
            cdl_error("ATRPC000 signature $1: the plugin option `clientCelltypeName` is not specified",
                signature.get_name)
        end

        unless @server_celltype_name
            cdl_error("ATRPC000 signature $1: the plugin option `serverCelltypeName` is not specified",
                signature.get_name)
        end
    end

    def set_server_celltype_name(value)
        @server_celltype_name = value
    end

    def set_client_celltype_name(value)
        @client_celltype_name = value
    end

    # This is where the plugin is supposed to generate extra CDL code
    #
    #file:: File:
    def gen_cdl_file(file)
        file.print "/* '#{self.class.name}' code */\n"

        # Generate the (un)marshaler celltype.
        marshaler_gen = MarshalerGenerator.new @signature
        marshaler_gen.gen_marshaler_celltype()
        file.print "import(<rpc/sChannel.cdl>);\n"
        file.print "import(<rpc/sRPCErrorHandler.cdl>);\n"
        file.print "import(<rpc/TDR.cdl>);\n"
        file.print "import(<rpc/RPCTask.cdl>);\n"
        file.print "import(\"#{marshaler_gen.cdl_file_name}\");\n"

        # Generate the client proxy celltype definition
        file.print "composite #{@client_celltype_name} {\n"
        file.print "  entry #{@signature.get_namespace_path} eEntry;\n"
        file.print "  call sChannel cChannel;\n"
        file.print "  \n"
        file.print "  cell #{marshaler_gen.marshaler_celltype_name} Marshal {\n"
        file.print "    cTDR = TDR.eTDR;\n"
        file.print "  };\n"
        file.print "  \n"
        file.print "  cell tTDR TDR {\n"
        file.print "    cChannel => composite.cChannel;\n"
        file.print "  };\n"
        file.print "  \n"
        file.print "  eEntry => Marshal.eClientEntry;\n"
        file.print "};\n"

        # Generate the server proxy celltype definition
        file.print "composite #{@server_celltype_name} {\n"
        file.print "  call #{@signature.get_namespace_path} cCall;\n"
        file.print "  call sChannel cChannel;\n"
        file.print "  entry sUnmarshalerMain eService;\n"
        file.print "  \n"
        file.print "  cell #{marshaler_gen.unmarshaler_celltype_name} Marshal {\n"
        file.print "    cTDR = TDR.eTDR;\n"
        file.print "    cServerCall => composite.cCall;\n"
        file.print "  };\n"
        file.print "  \n"
        file.print "  cell tTDR TDR {\n"
        file.print "    cChannel => composite.cChannel;\n"
        file.print "  };\n"
        file.print "  \n"
        file.print "  eService => Marshal.eService;\n"
        file.print "};\n"
    end

    #file:: File:
    def gen_post_code(file)
    end

    # > 受け口関数の本体(C言語)を生成する
    # > 通常であれば、ジェネレータは受け口関数のテンプレートを生成する
    # > プラグインの場合、変更する必要のないセルタイプコードを生成する
    #
    # It appears that this method gets called for every entry point of every
    # celltype generated by the plugin.
    #
    #file::           FILE        出力先ファイル
    #b_singleton::    bool        true if singleton
    #ct_name::        Symbol
    #global_ct_name:: string
    #sig_name::       string
    #ep_name::        string
    #func_name::      string
    #func_global_name:: string
    #func_type::      class derived from Type
    def gen_ep_func_body( file, b_singleton, ct_name, global_ct_name, sig_name, ep_name, func_name, func_global_name, func_type, params )
        marshaler_gen = MarshalerGenerator.new @signature
        marshaler_gen.gen_ep_func_body(file, b_singleton, ct_name, global_ct_name, sig_name, ep_name, func_name, func_global_name, func_type, params)
    end

    # It appears that this method gets called in a similar way to
    # `gen_ep_func_body`, but with a different output location.
    def gen_preamble file, b_singleton, ct_name, global_name
        marshaler_gen = MarshalerGenerator.new @signature
        marshaler_gen.gen_preamble file, b_singleton, ct_name, global_name
    end

    # It appears that this method gets called in a similar way to
    # `gen_ep_func_body`, but with a different output location.
    def gen_postamble file, b_singleton, ct_name, global_name
        marshaler_gen = MarshalerGenerator.new @signature
        marshaler_gen.gen_postamble file, b_singleton, ct_name, global_name
    end
end

# We just need a portion of `GenOpaqueMarshaler`. To avoid it from
# interfering with the existing members of `AtRPCClientProxyPlugin` (I don't
# know if this actually happens, but I took precaution just in case. Personally
# I'm starting to feel like having this mix-in thingy in the language
# specification is an awful idea), instantiate `GenOpaqueMarshaler` in a
# separate class.
#
# Since there are no annotations regarding accessibility, I'm going to assume
# *everything* inside `GenOpaqueMarshaler` is public and stable.
class MarshalerGenerator
    include GenOpaqueMarshaler
    include GenParamCopy

    def initialize(signature)
        @signature = signature
        self.initialize_opaque_marshaler
    end

    # Get the name of the CDL file including the definition of the marshaler
    # celltypes generated by `GenOpaqueMarshaler`.
    def cdl_file_name
        return "#{@marshaler_celltype_name}.cdl"
    end

    # Get the path to the marshaler celltype. The celltype looks like the
    # following:
    #
    #     celltype #{@marshaler_celltype_name} {
    #       entry #{@signature.get_namespace_path} eClientEntry;
    #       call sTDR       cTDR;
    #       [optional]
    #         call sSemaphore cLockChannel;
    #       [optional]
    #         call sRPCErrorHandler cErrorHandler;
    #     };
    #
    def marshaler_celltype_name
        @marshaler_celltype_name
    end

    # Get the path to the unmarshaler celltype. The celltype looks like the
    # following:
    #
    #     celltype #{@unmarshaler_celltype_name} {
    #       call #{@signature.get_namespace_path} cServerCall;
    #       call  sTDR       cTDR;
    #       [optional]
    #         call sRPCErrorHandler cErrorHandler;
    #       entry sUnmarshalerMain  eService;
    #     };
    #
    def unmarshaler_celltype_name
        @unmarshaler_celltype_name
    end

    # `GenOpaqueMarshaler` thinks `self` is an instance of a subclass of
    # `ThroughPlugin`, which has a method named `subst_name`. Pretend we are
    # `ThroughPlugin` so that the interpreter doesn't get furious when finding
    # out that `self` doesn't have `subst_name`.
    def subst_name hoge
        hoge
    end
end
