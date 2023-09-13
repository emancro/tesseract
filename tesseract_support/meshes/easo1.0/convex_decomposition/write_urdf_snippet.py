import glob
import xml.etree.ElementTree as ET

# <collision>
#     <geometry>
#         <mesh filename="package://tesseract_support/meshes/car_seat/collision/car_12.stl"/>
#     </geometry>
# </collision>


def write_urdf_snippet(load_path, output_filename):
    filesnames = glob.glob(load_path + "/*.stl")

    with open(output_filename, "wb") as f:
        for filename in filesnames:
            data = ET.Element('collision')
            

            element1 = ET.SubElement(data, 'geometry')
            
            s_elem1 = ET.SubElement(element1, 'mesh')

            
            s_elem1.set('filename', "package://tesseract_support/meshes/easo1.0/convex_decomposition/robot_base_decomposition_meshes/" + filename.split('/')[-1])
            
            b_xml = ET.tostring(data)
            
            # Opening a file under the name `items2.xml`,
            # with operation mode `wb` (write + binary)
            
            f.write((b_xml.decode() + '\n').encode())


if __name__ == "__main__":
    write_urdf_snippet("/home/febert/Desktop/decomp2", 'easo_robot_base_decomp.xml')

