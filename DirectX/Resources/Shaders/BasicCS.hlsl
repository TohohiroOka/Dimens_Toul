#define RS "RootFlags(ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT),"\
                    "DescriptorTable(UAV(u0, numDescriptors = 1, space = 0, offset = DESCRIPTOR_RANGE_OFFSET_APPEND), "\
                                    "visibility = SHADER_VISIBILITY_ALL),"\
                    "StaticSampler(s0, "\
                                  "filter         = FILTER_MIN_MAG_MIP_LINEAR, "\
                                  "addressU       = TEXTURE_ADDRESS_WRAP, "\
                                  "addressV       = TEXTURE_ADDRESS_WRAP, "\
                                  "addressW       = TEXTURE_ADDRESS_WRAP, "\
                                  "mipLodBias     = 0.0f, "\
                                  "maxAnisotropy  = 0, "\
                                  "comparisonFunc = COMPARISON_NEVER, "\
                                  "borderColor    = STATIC_BORDER_COLOR_TRANSPARENT_BLACK, "\
                                  "minLOD         = 0.0f, "\
                                  "maxLOD         = 3.402823466e+38f, "\
                                  "space          = 0, "\
                                  "visibility     = SHADER_VISIBILITY_ALL)"

struct Input
{
    float3 S_pos;
    float3 E_pos;
    float3 N_pos;
    uint time;
};

// 共有データ
RWStructuredBuffer<Input> real : register(u0);

[RootSignature(RS)]
[numthreads(1, 1, 1)]
void main(uint3 gID : SV_GroupID)
{
    //共有データに配列番号が入る(0~255)
    real[gID.x].S_pos = float3(100, 100, 100);
    real[gID.x].N_pos = float3(real[gID.x].S_pos.x + 100, real[gID.x].S_pos.y, real[gID.x].S_pos.z);
    real[gID.x].E_pos = real[gID.x].E_pos;
    real[gID.x].time += 1;
}